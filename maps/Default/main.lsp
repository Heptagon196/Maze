(main
    ;将maze.map中的坐标设置为墙
    (set Wall "maze.map")
    ;当到达坐标 (1, 2) 时触发事件
    (event 1 2
        (print "The exit is at the south-east corner.")
    )
    (event 1 15
        (if (== (get 39 18) Wall)
            (print "You found a key in the corner!")
            (set 39 18 Empty)
        )
    )
    (event 39 17
        (if (== (get 39 18) Wall)
            (print "The way is blocked! The key is near the south-west corner.")
        )
    )
    (event 40 19
        (print "You escaped from the maze! Press any key to exit.")
        (getch)
        (exit)
    )
)
