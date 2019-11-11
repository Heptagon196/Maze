# ConsoleRPG
走迷宫。目标是变成RPG。</br>
在 config.gos 中填写需使用的存放在 maps 目录下的地图(其中 'Editor' 是一个简单的编辑器，'Select' 为一个简单的启动菜单)。</br>
迷宫会解析指定地图目录下的脚本(事实上 Editor 与 Select 也是由此脚本编写，include 目录下则是编写的此脚本的库，详见 maps/Editor/main.gos 与 maps/Select/main.lsp)，这是一种效率极低的辣鸡脚本，语法可见[Gos](https://github.com/heptagon196/Gos)。</br>
通过查看 config.gos, maps/Editor/main.gos 和 maps/Default/main.gos，你可以大致了解目前实现的功能。
