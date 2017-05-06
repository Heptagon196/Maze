(main
    (def NewWall white blue "  " true)
    ;定义新方块，蓝色，可以穿过
    (clear)
    (unhidecursor)
    ;print语句会在第21行开始输出，因此调用echo命令
    (system "echo Input the map name: ")
    (string name)
    (read name)
    (hidecursor)
    (= name (+ (+ "./maps/" name) "/maze.map"))
    ;简单的文件操作。/dev/stdin只是代指stdin而已，并不仅限于linux
    (if (readfrom name)
        (readfrom "/dev/stdin")
    (else)
        (system (+ "echo File not found: " name))
        (exit)
    )
    ;读入name文件中的坐标，这些坐标的类型被设置为NewWall
    (source NewWall name)
    (print "Press r to change the block, press e to save the map.")
    ;定义按下r时的行为
    (key "r"
        (if (== (get (locx) (locy)) Empty)
            (set (locx) (locy) NewWall)
        (else)
            (set (locx) (locy) Empty)
        )
    )
    (key "e"
        (printto name)
        (for (= x 1) (<= x 40) (++ x)
            (for (= y 1) (<= y 20) (++ y)
                (if (== (get x y) NewWall)
                    (println x " " y)
                )
            )
        )
        (printto "/dev/stdout")
        (print "Map saved!")
    )
)
