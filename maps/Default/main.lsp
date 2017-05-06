(main
    ;将maze.map中的坐标设置为墙
    (source Wall "maze.map")
    ;当到达坐标 (1, 2) 时触发事件
    (event 1 2
        (print "The exit is at the south-east corner.")
    )
    (event 9 17
        (if (! (can-cross 38 19))
            (print "You found a key in the corner!")
            (set 38 19 Empty)
        )
    )
    (event 37 19
        (if (== (get 38 19) Wall)
            (print "The way is blocked! The key is near the south-west corner.")
        )
    )
    (event 40 20
        (print "You escaped from the maze! Press any key to exit.")
        (getch)
        (exit)
    )
)
