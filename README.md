# MAZE
走迷宫。也许会变成RPG(???)</br>
在maze.config中填写需使用的存放在maps目录下的地图(其中'Editor'是一个简单的编辑器)。</br>
迷宫会解析指定地图目录下的脚本(事实上Editor也是由此脚本编写，详见maps/Editor/main.lsp)，这是一种效率极低的辣鸡脚本(虽然后缀是lsp，但是除了有括号以外和Lisp完全不同……)，不过足以应付当前需求，以后将会重构。</br>
通过查看maps/Editor/main.lsp和maps/Default/main.lsp，你可以大致了解目前实现的功能。
