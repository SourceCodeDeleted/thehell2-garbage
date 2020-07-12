Update of 15/10/2005 : Minor corrections (transparency handling, selecting/canceling tile)


Second release. Some cosmetic bug corrections, and now loads correctly monster tables for most diablo versions, without having to write down various offsets.


Here are some utilities to edit Diablo/Hellfire dungeons. They are :
*edit_dun : *.dun editor
*join_dun : pack several *.dun into one. Usefull for editing the full town map at once, and for copy/paste between different area (join the 2 area, copy/paste, then split again).
*split_dun : make several *.dun from one. Reverse of above one.
*Extract_dun : For Hellfire only (at the moment, if anyone finds other dungeon parts I'll add them). Extract 2 *.dun from the *.exe, corresponding to the Nakrull room and the CornerStone of the World room.

Repacking *.dun into *.exe is planned too, see you soon ;)

All of these programs are using the same layout : You have to make a work directory (basically the dir/subdirs that are present in various *.MPQ), and to configure dun_edit.ini.

Work directory :
----------------
It should contain the following directories :
levels\
levels\towndata\
levels\l1data\
levels\l2data\
levels\l3data\
levels\l4data\
nlevels\
nlevels\towndata\
nlevels\l5data\
nlevels\l6data\

They are used for saving files (and you have to repack them yourself into MPQ if you want that the game uses them).
Programs will search first in this directory, then in the various *.MPQ when it looks for a file.
So at the beginning they just use Diablo/Hellfire files, but when you start to edit some of them, those alternate version are used. 

dun_edit.ini :
--------------
You *Must* setup the paths to MPQ files, and to your work directory.
You can change various settings related to windowed/fullscreen mode, size of screen, background color...
At last, if you want to visualize monsters, you have to setup the path to the game exe. Standart values shouldn't be changed unless you are modding (and what a mod, moving monster tables !).

Arguments of programs :
-----------------------
edit_dun.exe file.dun act
where "file.dun" is the name of the file, without its path, with ".dun" extension; and "act" is a number indicating the area :
0 : town
1 : church
2 : catacombs
3 : caves
4 : Hell
5 : Crypt
6 : Nest
7 : Hellfire's town

Exemple : edit_dun.exe vile1.dun 1

Extract_dun.exe has no argument (but requires correct dun_edit.ini).

join_dun.exe and split_dun requires an extra file indicating what should be done, look at the join_dun.ini or split_dun.ini for an example. (last argument in split_dun.ini is the number of extra layers, usually 4).


Commands for the editor :
-------------------------
No mouse at the moment...

Move the window with arrows, or Shift-Arrow to go faster

Move cursor with 1/3/7/9 of the numeric pad

Select area with Shift-1..Shift-9 of the numeric pad

Change palette with 1/2/3/4/5/6

Change display flags with F1..F5

Change gamma with F12/Shift-F12

Zomm in/out with =/- keys (qwerty keybord)

Change spacing between tiles with a/z (qwerty keybord)

Change mode of editing with tab (edit the tile, or one of the extra layers, including monsters)

Change what is under the cursor with Enter, then choose with arrow, valid/quit with Enter/Esc

Copy/cut/paste with CTRL-C/CTRL-X/CTRL-V (works in map mode and in monster mode)

Quit with Esc

Snapshot with P

Center map with Space

Save with CTRL-S

H for this help
