(main
    ;method指定显示方式。0 代表全部显示，1 代表显示探索过的方块，2 代表显示视野内的方块
    (= method 2)
    ;horizon指定视野范围，默认为2。
    (= horizon 2)
    ;将maze.map中的坐标设置为墙
    (set Wall "maze.map")
    ;当到达坐标 (1, 2) 时触发事件
    (event 1 2 0
        (print "出口在右下角。")
    )
    (event 1 15 0
        (if (== (get 39 18) Wall)
            (print "你找到了一把钥匙！")
            (set 39 18 Empty)
        )
    )
    (event 39 17 0
        (if (== (get 39 18) Wall)
            (print "路被堵住了，出口在左下角。")
        )
    )
    (int fl)
    (event 40 19 0
        (if (== fl 0)
            (print "你走进了另一个迷宫。")
            (addfloor)
            (set Wall "maze2.map")
            (setloc 1 1 1)
            (++ fl)
        (else)
            (setloc 1 1 1)
        )
    )
    (event 40 19 1
        (print "你走出了迷宫！按任意键退出。")
        (getch)
        (exit)
    )
)
