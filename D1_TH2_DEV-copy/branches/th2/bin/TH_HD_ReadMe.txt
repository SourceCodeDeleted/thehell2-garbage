The Hell 1 HD RC2 rev.1960 readme
--------------------------------

TH 1 HD is a project to rewrite The Hell in modern C++.
Once we achieve all features of actual mod version working,
we will be able to add support for high-resolution, mobile gaming,
cross platform client-server multiplayer and other cool things.


Instructions:
-------------

1. Launch by running TH1.exe

2. You can copy and rename save files from TH 1 to load them into TH 1 HD. For example:
	
	hellsp_0.hsv -> hell_hd_sp_0.hsv
	hellmp_0.drv -> hell_hd_mp_0.drv

	and stash file:

	Hell_SP.hst -> Hell_SP.hs2
	Hell_MP.hst -> Hell_MP.hs2


3. Configure by altering settings in config.ini. There are some useful options:
-------------------------------------------------------------------------------

	3.1. Changes screen resolution. For non-windowed fullscreen you must set only hardware supported resolution.

		set screenwidth 640
		set screenheight 480

	3.2. Startup windows state. You can switch fullscreen mode by pressing Alt + Enter:
	     For windowed fullscreen mode set win[off/on] mode.

		set fullscreen off/on/win/winon/winoff

	3.3. Set maximum fps rendering and show it. You can switch fps on the fly by Alt + F

		set fps 20/40/60/80
		set showfps on/off

	3.4  Set v-sync (default on, for G-Sync monitor may be needs off)

		set vsync on/off

	3.5. Whether to launch title video on start or not:

		set startvideo off/on

	3.6. Allow saving and returning to town in battle:

		set savealways off/on

	3.7. Launch TH 1 HD or TH 1 to compare behavior in the same renderer:

		set start th2/th1

	3.8. Turn on TH 1 stash using

		set stash on/off		

	3.9. Turn on zero cost spell casting and give all spells (works only in special two-window testing mode).
		 You can switch it on the fly by Alt + S :
		
		set freespell on/off

	3.10. Turn on god mode (works only in special two-window testing mode).
		  You can switch it on the fly by Alt + G :
		
		set godmode on/off

	3.11. Turn on easy autoclick mode. You can auto attack and auto cast spells without Shift key:
		
		set easyclick on/off

	3.12. Turn on auto pickup gold.  You can switch it on the fly by Alt + A :
		
		set autopickup on/off

	3.13. Save files are stored in \save_sp and \save_mp folders

		set savefolder on/off

	3.14. Turn on automatic backup for game saves
		
		set autobackup on/off

4. Parameters in config.ini for special TH 1 HD testing/comparing mode:
--------------------------------------------------

	When you run game in testing mode you see both TH playing at the same time on screen.
	You can just play game as usual, and then program noticed any difference in memory it stops
	and write two log files: th1.log and th2.log, which you have to send to developers to fix the error.
	You can send them to TheHell@support.assembla.com with description of gameplay situation just before the break,
	or create new support request on https://app.assembla.com/spaces/TheHell/support/tickets (anonymous posts with file attachment are allowed).
	
	You can use God Mode (Alt + G) and Free Spells (Alt + S) only in this testing mode.
	
	To turn on testing mode you can set or add this essential options in config.ini file:
		
		set exevalidation on
		set autocopysave on
		set loadonstart [testing_save_file_index]
		set difficulty [testing_difficulty]
		set newgame on/off	

	for example:

		set exevalidation on
		set autocopysave on
		set loadonstart m0 //multi player hellmp_0.drv testing
		set difficulty 2 //doom difficulty
		set newgame on //does not matter in multi player

	or:

		set exevalidation on
		set autocopysave on
		set loadonstart s0 //single player  hellsp_0.hsv testing
		set difficulty 0 //horror difficulty
		set newgame on //start new game in single player

	or:

		set exevalidation on
		set autocopysave on
		set loadonstart s1 //multiplayer hellsp_1.hsv testing
		set difficulty 1 //purgatory difficulty, but does not matter when loading single player game
		set newgame off //load saved game in single player
	
	Description for this options:

	4.1. Turns on simultaneously THs mode, TH 1 and TH 1 HD launch together in two windows at the same time:
		 (TH 1 in the top window, TH 1 HD in the bottom window)

		set exevalidating on/off

	4.2. Loads savefile specified by slot number on start (skips main menu):
		 For example: "set loadonstart s7" will load hellsp_7.hsv in TH 1 and hell_hd_sp_7.hsv in TH 1 HD.
		 This parameter is recommended to use in two-windows mode, because TH meta-menu doesn't support multithreading.
		 Also this parameter works in standalone mode when you just playing TH 1 HD or TH 1.

		set loadonstart off/s0-9/m0-9

	4.3. Set difficulty for multiplayer auto load.

		set difficulty 0-2

	4.4. Turn on automatic synchronization overwriting th2 save file with th1 on start (including stash file)
		
		set autocopysave on/off

	4.5. Load or renew single player game on start

		set newgame on/off	

	4.6. Dispose windows horizontaly

		set vertical off/on

----------------------------------------------------------
Submit your feedback and reports about any problems found:

https://app.assembla.com/spaces/TheHell/support/tickets/

http://thmod.com/
