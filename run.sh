##clear && echo "Building..." && /usr/bin/make -f /home/bios/CLionProjects/Game/Makefile -C /home/bios/CLionProjects/Game default_target && echo "##########################################Build finished!#################################" && echo "##########################################Launch the Application#################################" && ./Game
clear && echo "Building..." && /usr/bin/make -f /home/bios/CLionProjects/Game/Makefile -C /home/bios/CLionProjects/Game CFLAGS="-g" default_target && echo "##########################################Build finished!#################################" && echo "##########################################Launch the Application#################################" && ./Game
