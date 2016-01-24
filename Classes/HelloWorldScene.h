#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    bool m_gameOver;//游戏是否结束
    int score;
    int spDir;//表示精灵的方向 1，up 2 down
    int speed;//跳跃速度
    int m_level;//关卡编号
    bool jump;//表示是否精灵在跳跃，如果跳跃不处理屏幕的触摸
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void addScore(int r,int c,int n);
    void gameOver();
    void loadLevel(int level);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    void update(float t);
};

#endif // __HELLOWORLD_SCENE_H__
