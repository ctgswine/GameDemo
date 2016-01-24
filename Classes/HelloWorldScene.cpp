#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    m_level=1;//从第一关开始
    loadLevel(m_level);
    auto adds=Label::createWithBMFont("futura-48.fnt","");
    this->addChild(adds,103);
    adds->setTag(1200);
    adds->setPosition(Vec2(Director::getInstance()->getWinSize().width/2,
                           Director::getInstance()->getWinSize().height/2));
    ///加入精灵
    Vector<SpriteFrame* >allf;
    for(int i=1;i<7;i++)
    {
        auto sf=SpriteFrame::create(StringUtils::format("run%d.png",i), Rect(0,0,100,100));
        allf.pushBack(sf);
    }
    auto an=Animation::createWithSpriteFrames(allf);
    an->setDelayPerUnit(0.03);
    auto ani=Animate::create(an);
    auto Sp=Sprite::create();
    Sp->setTag(110);
    Sp->runAction(RepeatForever::create(ani));
    this->addChild(Sp,101);
    Sp->setPosition(300, 50);
    this->scheduleUpdate();
    jump=false;
    spDir=0;
    speed=30;
    m_gameOver=false;
    //加入分数
    auto labscore=Label::createWithBMFont("futura-48.fnt","0");
    this->addChild(labscore,102);
    labscore->setTag(111);
    labscore->setPosition(Director::getInstance()->getWinSize().width-200,Director::getInstance()->getWinSize().height/2);
    this->score=0;
    
    //加入触摸的处理
    auto listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=[&](Touch * t,Event * e){
        if (m_gameOver) {
            auto labend=(Label *)this->getChildByTag(1200);
            labend->setString("");
            jump=false;
            spDir=0;
            speed=30;
            m_gameOver=false;
            score=0;
            this->getChildByTag(10)->setPositionX(0);
            this->getChildByTag(110)->setPosition(300, 50);
            this->scheduleUpdate();
            
        }else
        {
            //        CCLOG("backlayer jump%d col%f tid=%d",jump);
            
            if (!this->jump) {//如果没有跳起
                jump=true;
                spDir=1;//向上
            }
        }
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}
void HelloWorld::loadLevel(int l)
{   //释放上一关资源
    auto oldmap=this->getChildByTag(10);
    if(oldmap!=nullptr)
    {
        oldmap->removeFromParentAndCleanup(true);
    }
    //添加地图层
    auto map=TMXTiledMap::create(StringUtils::format("map0%d.tmx",l));
    map->setTag(10);
    this->addChild(map,100);
    
    CCLOG("map size width %f",map->getMapSize().width);
    CCLOG("map size height%f",map->getMapSize().height);
    CCLOG("map getTileSize width%f",map->getTileSize().width);
    CCLOG("map getTileSize height%f",map->getTileSize().height);
    //获取子图层
    //    auto backlayer=map->getLayer("coin");
        //获取子图层某个行列的编号
    //    auto tid=backlayer->getTileGIDAt(Vec2(6, 12));
    //    CCLOG("coin row col tid=%d",tid);
    
}
void HelloWorld::update(float t)
{
    auto sp=this->getChildByTag(110);
    auto map=(TMXTiledMap *)getChildByTag(10);
    /////////////实现精灵和地图层碰撞
    
    
    //处理精灵的跳跃
    if (jump) {
        if (spDir==1) {
            sp->setPositionY(sp->getPositionY()+speed);
            speed-=2;
            if(speed<=0)
            {
                spDir=2;
            }
        }else if(spDir==2)
        {
            bool check=false;
            for (int i=0; i<speed; i+=2) {
                
                //1计算当前精灵相对于地图的坐标
                float px=sp->getPositionX();
                float py=sp->getPositionY()-sp->getContentSize().height/2+i;
                float mx=abs(map->getPositionX());//地图相对于屏幕的x坐标
                float my=0;
                float mapx=px+mx;//精灵相对于地图的x坐标
                float mapy=py+my;
                //2.获取精灵脚下的地图块的编号
                if (mapy/32>=0) {
                    
                    int gid=map->getLayer("back")->getTileGIDAt(Vec2((int)(mapx/32),(int)(19-mapy/32)));
                    //            if((int)(mapx/32)>=82)
                    //            {
                    //                CCLOG("gid %d col %d row%d",gid,(int)(mapx/32),(int)(19-mapy/32));
                    //
                    //            }
                    if (gid==66) {
                        check=true;//落到台阶上了
                        sp->setPositionY((int)(py+sp->getContentSize().height/2));
                        CCLOG("gid %d col %d row%d",gid,(int)(mapx/32),(int)(19-mapy/32));
                        check=true;
                        speed=30;
                        spDir=0;
                        jump=false;
                        break;
                    }
                }
                if (py<=-100) {
                    //游戏结束
                    gameOver();
                }
            }
            if(!check)
            {
                sp->setPositionY(sp->getPositionY()-speed);
                speed+=2;
                if(speed>30)
                {  speed=30;
                    //  spDir=0;
                    //  jump=false;
                }
            }
        }
        
        
    }
    this->getChildByTag(10)->setPositionX(this->getChildByTag(10)->getPositionX()-3);
    //地图的宽度 像素
    int mapWidth=map->getMapSize().width*map->getTileSize().width;
    int screenW=Director::getInstance()->getWinSize().width;
    if(map->getPositionX()<-(mapWidth-screenW))  //滚动背景
    {
        map->setPositionX(-(mapWidth-screenW));
        //移动角色
        sp->setPositionX(sp->getPositionX()+3);
        if(sp->getPositionX()>screenW-100)
        {
            sp->setPositionX(screenW-100);
        }
    }
    
    
    //1计算当前精灵相对于地图的坐标
    float px=sp->getPositionX();
    float py=sp->getPositionY()-sp->getContentSize().height/2;
    float mx=abs(map->getPositionX());//地图相对于屏幕的x坐标
    float my=0;
    float mapx=px+mx;//精灵相对于地图的x坐标
    float mapy=py+my;
    //2.获取精灵脚下的地图块的编号
    if(mapy/32>=0)
    {
        int gid=map->getLayer("back")->getTileGIDAt(Vec2((int)(mapx/32),(int)(19-mapy/32)));
        // CCLOG("backlayer row%f col%f tid=%d",mapy/32,19-mapx/32,gid);
        
        if ((jump==false)&&(spDir==0)&& (gid!=66)) {
            CCLOG("jumpjumpjump jump%d ",jump);
            
            jump=true;
            spDir=2;//下落
            speed=2;
        }
        //判断是否过关
        if(gid==6)
        {
            //     切换下一关
            m_level++;
            if(m_level<3)
            {
                loadLevel(m_level);
            }else
            {
                CCLOG("通关");
            }
        }
        //处理人物和金币层的碰撞
            int cid=map->getLayer("coin")->getTileGIDAt(Vec2((int)mapx/32,(int)(19-mapy/32)));
         CCLOG("coin row%f col%f cid=%d",19-mapy/32,mapx/32,cid);
            if (cid>0)
            {   Value s=map->getPropertiesForGID(cid);
                ValueMap m = s.asValueMap();
        
                addScore(px, py, m.at("s").asInt());
              //让金币消失
                map->getLayer("coin")->removeTileAt(Vec2((int)(mapx/32),(int)(19-mapy/32)));
            }
        
    }
}
void HelloWorld::addScore(int x,int y,int s)//加分
{   auto labscore=(Label *)this->getChildByTag(111);
    score+=s;//加分
    labscore->setString(StringUtils::format("%d",score));
    auto adds=Label::createWithBMFont("futura-48.fnt",
                                      StringUtils::format("%d",s));
    this->addChild(adds);
    adds->setPosition(Vec2(x,y));
    auto act1=Spawn::create(
                            MoveBy::create(1, Vec2(0,50)),
                            ScaleBy::create(1, 0.5),
                            FadeTo::create(1, 0),
                            nullptr
                            );
    auto act2=Sequence::create(act1,
                               CallFuncN::create([](Node * obj){
        obj->removeFromParentAndCleanup(true);
    }),
                               NULL);
    adds->runAction(act2);
}
void HelloWorld::gameOver()//游戏结束
{   m_gameOver=true;
    auto labend=(Label *)this->getChildByTag(1200);
    labend->setString("Game OVER");
    
    // this->stopAllActions();
    this->unscheduleAllSelectors();
    //this->getChildByTag(110)->stopAllActions();
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
