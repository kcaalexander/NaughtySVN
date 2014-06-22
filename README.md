##**NaughtySVN**

**NaughtySVN (NSVN) is an easy-to-use GUI based Subversion client which works as an extension to Nautilus File Manager (Graphical shell for the GNOME desktop environment). Inspired by TortoiseSVN, Its an effort primarily for non-technical people to use subversion.**

NSVN has a graphical interface which can be activated from Nautilus file browsing window. Its graphical windows are designed to be simple and easy to use, which  will allow users to create and administer subversion repository, checkout and commit working copies, handling working copies, browsing repositories and working copies, handling remote repositories and working copies etc.  



###Building NaughtySVN

See ./INSTALL for a details building and installation document. Following is a simple way to build and install NSVN. Subversion include and library
paths are all  hard coded, so in case of problems please make appropriate changes in ./svn/Makefile.am.

   1. 'cd' to top most directory of NSVN and type './autogen.sh'

   2. 'make' to compile NSVN

   3. 'make install' to install NSVN.



###Directory Layout

*doc/*
   User and Developer documentation build using doxygen.
*m4/*
   Custom GNU build system macro processors.
*notes/*
   Developers docs and scribbling location.
*packages/*
   Stuff to help packaging systems, like rpm and dpkg.
*pixmaps/*
   Images for GUI.
*po/*
   Translatable strings for Multi-language support.
*www/*
   NaughtySVN home page, live content at www.naughtysvn.org
*nautilus/*
   Integration with Nautilus.
*nsvn/*
   User interface and GUI's
*svn/*
   NaughtySVN Subversion binding API.



###Invocation/Running

NaughtySVN is tightly coupled with subversion and nautilus and is not designed to invoke it separately from command line. But in the interest
of developers and to test svn and nsvn modules faster, they can call the 'naughtysvn' executable from command line use following format

   **_naughtysvn MID=NSVN CMD=command list_of_arguments ..._**

   where 'MID=NSVN' is a magic id used to transfer NSVN related data from Nautilus to NSVN
         'command' is the command to be executed by nsvn
         'list_of_arguments' are list of arguments need to execute the command separated by space.
         
   for example:
         *naughtysvn MID=NSVN CMD=create_repos /tmp/repos*



###Authors

 - **The LibGit2Sharp [contributors][2]
 - **Logo:** [Jason "blackant" Long][18]

 [2]: https://github.com/kcaalexander/NaughtySVN/blob/master/AUTHORS



###License

NaughtySVN is licensed under GPL2 (Refer to the [LICENSE.md][3] file)

 [3]: https://github.com/kcaalexander/NaughtySVN/blob/master/LICENSE.md

##Further Information

The official NaughtySVN homepage is located at tigris.org[0]

[0]http://naughtysvn.tigris.org
