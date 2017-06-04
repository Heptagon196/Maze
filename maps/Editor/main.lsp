(main
    (devmode.off)
    (map.level 0)
    (map.def NewWall white blue "  " true)
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
    (map.set NewWall name)
    (print "按下r改变方块，e保存地图，上下左右键批量复制当前方块")
    ;定义按下r时的行为
    (key "r"
        (if (== Empty (map.get (map.x) (map.y)))
            (map.set NewWall (map.x) (map.y))
        (else)
            (map.set Empty (map.x) (map.y))
        )
    )
    (int i)
    (key "BPj"
        (int type)
        (if (== (map.get (map.x) (map.y)) NewWall)
            (= type NewWall)
        (else)
            (= type Empty)
        )
        (for (= i (map.y)) (<= i 20) (++ i)
            (map.set type (map.x) i)
        )
    )
    (key "AHk"
        (int type)
        (if (== (map.get (map.x) (map.y)) NewWall)
            (= type NewWall)
        (else)
            (= type Empty)
        )
        (for (= i (map.y)) (>= i 1) (-- i)
            (map.set type (map.x) i)
        )
    )
    (key "DKh"
        (int type)
        (if (== (map.get (map.x) (map.y)) NewWall)
            (= type NewWall)
        (else)
            (= type Empty)
        )
        (for (= i (map.x)) (>= i 1) (-- i)
            (map.set type i (map.y))
        )
    )
    (key "CMl"
        (int type)
        (if (== (map.get (map.x) (map.y)) NewWall)
            (= type NewWall)
        (else)
            (= type Empty)
        )
        (for (= i (map.x)) (<= i 40) (++ i)
            (map.set type i (map.y))
        )
    )
    (key "e"
        (printto name)
        (for (= x 1) (<= x 40) (++ x)
            (for (= y 1) (<= y 20) (++ y)
                (if (== (map.get x y) NewWall)
                    (println x " " y)
                )
            )
        )
        (printto "/dev/stdout")
        (print "Map saved!")
    )
)
