# MD5Arena

本项目是MD5竞技场的简化版, 用作“一生一芯”第三期的个性化测试项目. 
由于第三期芯片不要求cache, 性能较差, 因此只设计了命令行. 

## 编译方法

若没有`NAVY_HOME`, 直接执行

```
make run
```

即可. 

若已有`NAVY_HOME`, 可以符号链接到`$NAVY_HOME/apps`下使用.

## 属性与技能

属性包括:
- HP
- ATT, 攻击
- DEF, 防御
- SPD, 速度
- DEX, 敏捷
- MAG, 魔法
- RES, 抗性
- LER, 智力

这些属性影响战斗的结果. 

技能详见`include/skills.h`. 

## 可用命令

- `help`, 显示帮助
- `add`, 添加玩家
- `rm`, 删除玩家
- `ls`, 列出现有玩家
- `start`, 开始游戏
- `exit`, 退出MD5竞技场 