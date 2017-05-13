(main
    ;method指定显示方式。0 代表全部显示，1 代表显示探索过的方块，2 代表显示视野内的方块
    (= method 2)
    ;horizon指定视野范围，默认为2。
    (= horizon 2)
    ;将maze.map中的坐标设置为墙
    (set Wall "maze.map")
    ;当到达坐标 (1, 2) 时触发事件
    (event 1 2
        (print "出口在右下角。")
    )
    (event 1 15
        (if (== (get 39 18) Wall)
            (print "你找到了一把钥匙！")
            (set 39 18 Empty)
        )
    )
    (event 39 17
        (if (== (get 39 18) Wall)
            (print "路被堵住了，出口在左下角。")
        )
    )
    (event 40 19
        (print "你逃离了迷宫！按任意键退出。")
        (getch)
        (exit)
    )
)
