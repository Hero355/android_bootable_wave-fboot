/*
 ; This file is part of Badadroid project.
 ;
 ; Copyright (C) 2012 Rebellos, mijoma, b_kubica
 ;
 ;
 ; Badadroid is free software: you can redistribute it and/or modify
 ; it under the terms of the GNU General Public License as published by
 ; the Free Software Foundation, either version 3 of the License, or
 ; (at your option) any later version.
 ;
 ; Badadroid is distributed in the hope that it will be useful,
 ; but WITHOUT ANY WARRANTY; without even the implied warranty of
 ; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ; GNU General Public License for more details.
 ;
 ; You should have received a copy of the GNU General Public License
 ; along with Badadroid.  If not, see <http://www.gnu.org/licenses/>.
 ;
 ;
*/
#include "string.h"
#include "BL3.h"
#include "atag.h"

int main(runMode_t mode)
{
   if (mode==rm_FOTA_INTERACTIVE){
   void* kernelImage = L""; // g - internal sd card, e - external sd card
   char* cmdlnRM = "bootmode=2 loglevel=4";
   char* cmdln = "loglevel=4";

   unsigned char ATAG_buf[512]={0};
   t_stat filestat;
   fun_t kernel;
   int fd;
   unsigned long kernelSize=0;

   //here we start the real deal :)

   int mmuctrl = MemMMUCacheEnable(gMMUL1PageTable, 1);

   disp_FOTA_Init();
   __PfsNandInit();
   __PfsMassInit();
   MemoryCardMount();

//DISPLAYFUNCTIONS
    char entry1[30];
    char entry2[30];
    char entry3[30];
    char title[30];
    void RefreshScreen(){
        int i;
        for (i=0;i<255-24;i++){
            disp_FOTA_Printf(" ");
            }
            disp_FOTA_Init();
    }
    void drawEntry(int entry,char entry1[30],char entry2[30],char entry3[30],char title[30]){
        int i;
        switch(entry)
        {
            case 0:
            disp_FOTA_Printf("                              ");
            disp_FOTA_Printf(title);
            for(i=1;i<4;i++){disp_FOTA_Printf("                              ");}
            disp_FOTA_Printf("*----------------------------*");
            disp_FOTA_Printf(entry1);
            disp_FOTA_Printf("*----------------------------*");
            for(i=7;i<12;i++){disp_FOTA_Printf("                              ");}
            disp_FOTA_Printf(entry2);
            for(i=13;i<19;i++){disp_FOTA_Printf("                              ");}
            disp_FOTA_Printf(entry3);
            for(i=20;i<24;i++){disp_FOTA_Printf("                              ");}
                break;
            case 1:
            disp_FOTA_Printf("                              ");
            disp_FOTA_Printf(title);
            for(i=1;i<5;i++){disp_FOTA_Printf("                              ");}
            disp_FOTA_Printf(entry1);
            for(i=6;i<11;i++){disp_FOTA_Printf("                              ");}
            disp_FOTA_Printf("*----------------------------*");
            disp_FOTA_Printf(entry2);
            disp_FOTA_Printf("*----------------------------*");
            for(i=14;i<19;i++){disp_FOTA_Printf("                              ");}
            disp_FOTA_Printf(entry3);
            for(i=20;i<24;i++){disp_FOTA_Printf("                              ");}
                break;
            case 2:
            disp_FOTA_Printf("                              ");
            disp_FOTA_Printf(title);
            for(i=1;i<5;i++){disp_FOTA_Printf("                              ");}
            disp_FOTA_Printf(entry1);
            for(i=6;i<12;i++){disp_FOTA_Printf("                              ");}
            disp_FOTA_Printf(entry2);
            for(i=13;i<18;i++){disp_FOTA_Printf("                              ");}
            disp_FOTA_Printf("*----------------------------*");
            disp_FOTA_Printf(entry3);
            disp_FOTA_Printf("*----------------------------*");
            for(i=21;i<24;i++){disp_FOTA_Printf("                              ");}
                break;
        }
        }


//DISPLAYMENU
int countStop=0;
menu1:
kernelImage = L"/e/boot.img";
int currentMenu=0;
int pass=0;
int count=0;
int bootMode=0;
disp_FOTA_Init;
//Tests
    strcpy(entry1,"     Boot Android Kernel 1    ");
    strcpy(entry2,"     Boot Android Kernel 2    ");
    strcpy(entry3,"          Boot Bada           ");
    strcpy(title,"  Welcome to Fota Bootloader  ");
    drawEntry(currentMenu,entry1,entry2,entry3,title);
    while(pass==0&&count<500000){

    if(is_key_pressed(1,1)){
        currentMenu=currentMenu+1;
        if (currentMenu>2){currentMenu=0;};
        countStop=1;
        RefreshScreen();
        drawEntry(currentMenu,entry1,entry2,entry3,title);}
       while(is_key_pressed(1,1)){}
     if(is_key_pressed(1,2)){
         countStop=1;
        currentMenu=currentMenu-1;
        if (currentMenu<0){currentMenu=2;};

        RefreshScreen();
        drawEntry(currentMenu,entry1,entry2,entry3,title);}
       while(is_key_pressed(1,2)){}
    if(is_key_pressed(0,0)){
        pass=1;}
        if(countStop==0){count=count+1;}
}
    RefreshScreen();
    if(pass==1)//Display second menu
    {
        pass=0;
    if(currentMenu==2){return rm_BL3;}
    else if(currentMenu==1){
        strcpy(title,"  Booting kernel 2 in mode ?  ");
        kernelImage = L"/e/boot_2.img";
}
    else{
        strcpy(title,"  Booting kernel 1 in mode ?  ");}

    strcpy(entry1,"      Boot in normal mode     ");
    strcpy(entry2,"      Boot Recovery mode      ");
    strcpy(entry3,"           Go Back            ");

    drawEntry(currentMenu,entry1,entry2,entry3,title);
    while(pass==0){

    if(is_key_pressed(1,1)){
        currentMenu=currentMenu+1;
        if (currentMenu>2){currentMenu=0;};
        RefreshScreen();
        drawEntry(currentMenu,entry1,entry2,entry3,title);}
       while(is_key_pressed(1,1)){}
     if(is_key_pressed(1,2)){
        currentMenu=currentMenu-1;
        if (currentMenu<0){currentMenu=2;};
        RefreshScreen();
        drawEntry(currentMenu,entry1,entry2,entry3,title);}
       while(is_key_pressed(1,2)){}
    if(is_key_pressed(0,0)){
        RefreshScreen();
        pass=1;}

}
if(currentMenu==0){bootMode=0;}
else if(currentMenu==1){bootMode=1;}
else{goto menu1;}

}
   tfs4_stat(kernelImage, &filestat);
   kernelSize = filestat.st_size;


   if ((kernelSize > 0) && ((fd=tfs4_open(kernelImage, 4)) >= 0))
   {
      tfs4_read(fd, &KERNEL_BUF, kernelSize);
      tfs4_close(fd);

      DisableMmuCache(mmuctrl);
      _CoDisableMmu();
      disp_FOTA_Printf("       Booting Modem       ");

      DRV_Modem_BootingStart();
      disp_FOTA_Printf("                             ");
      disp_FOTA_Printf("        Init Modem           ");

      setup_core_tag(ATAG_buf);
      setup_serial_tag(0x123, 0x456);
      setup_rev_tag('0');
      /*if (mode == rm_FOTA_RECOVERY)
      {
         setup_cmdline_tag(cmdlnRM);
         disp_FOTA_Printf("                             ");
         disp_FOTA_Printf("     Boot in Recovery Mode   ");
      }
      else if (mode == rm_FOTA_RECOSECOND)
      {
         setup_cmdline_tag(cmdlnRM);
         disp_FOTA_Printf("                             ");
         disp_FOTA_Printf("    Boot 2 in Recovery Mode  ");
      }
      else if (mode == rm_FOTA_SECOND)
       {
         setup_cmdline_tag(cmdln);
         disp_FOTA_Printf("                             ");
         disp_FOTA_Printf("     Boot 2 in Normal Mode   ");
      }
      else
      {
         setup_cmdline_tag(cmdln);
         disp_FOTA_Printf("                             ");
         disp_FOTA_Printf("      Boot in Normal Mode    ");
      }*/


    if(bootMode==0){setup_cmdline_tag(cmdln);}
    else{setup_cmdline_tag(cmdlnRM);}

      setup_end_tag();

      //copy kernel to the right position
      memcpy(&KERNEL_START, &KERNEL_BUF, kernelSize);
      disp_FOTA_Printf("                             ");
      disp_FOTA_Printf("    Copied kernel to boot     ");
      disp_FOTA_Printf("            Wait!             ");

      //SYSCON operations
      *((unsigned int*)SYSCON_NORMAL_CFG) = 0xFFFFFFFF;
      _CoDisableDCache();
      _System_DisableVIC();
      _System_DisableIRQ();
      _System_DisableFIQ();

      kernel = (fun_t)&KERNEL_START;
      kernel(0, &ATAG_MODEL, ATAG_buf);

   }
   else
   {
      disp_FOTA_Printf("                              ");
      disp_FOTA_Printf("            ERROR:            ");
      disp_FOTA_Printf(" boot.img not found or broken ");
      disp_FOTA_Printf("                              ");
      disp_FOTA_Printf("   Please re-copy boot.img    ");
      disp_FOTA_Printf("     to external SD card      ");
   }

   //loop forever
   while(1);

   return 0;
   }
   else {
       void* kernelImage = L""; // g - internal sd card, e - external sd card
   char* cmdlnRM = "bootmode=2 loglevel=4";
   char* cmdln = "loglevel=4";

   unsigned char ATAG_buf[512]={0};
   t_stat filestat;
   fun_t kernel;
   int fd;
   unsigned long kernelSize=0;

   //here we start the real deal :)

   int mmuctrl = MemMMUCacheEnable(gMMUL1PageTable, 1);

   if (mode == rm_FOTA_RECOSECOND || mode == rm_FOTA_SECOND)
     disp_First_Image();//    - white screen if secondary boot
    else
       disp_First_Image();//  - black screen else
   /*disp_FOTA_Printf("                              ");
   disp_FOTA_Printf("                              ");
   disp_FOTA_Printf("       XDA DEVELOPERS         ");
   disp_FOTA_Printf("                              ");
   disp_FOTA_Printf("                              ");
   disp_FOTA_Printf(" __      __                   ");
   disp_FOTA_Printf("/  \\    /  \\____ ___  __ ____ ");
   disp_FOTA_Printf("\\   \\__/   |__  \\\\  \\/ // __ \\");
   disp_FOTA_Printf(" \\        / / __ \\\\   / \\ ___/");
   disp_FOTA_Printf("  \\__/\\__/ (_____/ \\_/   \\___");
   disp_FOTA_Printf("                              ");
   disp_FOTA_Printf("   ,(   ,(   ,(   ,(   ,(  ,( ");
   disp_FOTA_Printf(" `-   `-   `-   `-   `-   `-  ");
   disp_FOTA_Printf("                              ");
   disp_FOTA_Printf("                              ");
*/
   __PfsNandInit();
   __PfsMassInit();
   MemoryCardMount();
  // disp_FOTA_Printf("     Mounted partitions       ");

   if(mode == rm_FOTA_RUN)
     kernelImage = L"/e/boot.img";
   if(mode == rm_FOTA_RECOVERY)
     kernelImage = L"/e/boot.img";
   if(mode == rm_FOTA_SECOND)
     kernelImage = L"/e/boot_2.img";
   if(mode == rm_FOTA_RECOSECOND)
     kernelImage = L"/e/boot_2.img";


   tfs4_stat(kernelImage, &filestat);
   kernelSize = filestat.st_size;

   if ((kernelSize > 0) && ((fd=tfs4_open(kernelImage, 4)) >= 0))
   {
      tfs4_read(fd, &KERNEL_BUF, kernelSize);
      tfs4_close(fd);

      DisableMmuCache(mmuctrl);
      _CoDisableMmu();

      DRV_Modem_BootingStart();
    //  disp_FOTA_Printf("                             ");
      //disp_FOTA_Printf("        Init Modem           ");

      setup_core_tag(ATAG_buf);
      setup_serial_tag(0x123, 0x456);
      setup_rev_tag('0');
      if (mode == rm_FOTA_RECOVERY)
      {
         setup_cmdline_tag(cmdlnRM);
        // disp_FOTA_Printf("                             ");
         //disp_FOTA_Printf("     Boot in Recovery Mode   ");
      }
      else if (mode == rm_FOTA_RECOSECOND)
      {
         setup_cmdline_tag(cmdlnRM);
        // disp_FOTA_Printf("                             ");
         //disp_FOTA_Printf("    Boot 2 in Recovery Mode  ");
      }
      else if (mode == rm_FOTA_SECOND)
       {
         setup_cmdline_tag(cmdln);
         //disp_FOTA_Printf("                             ");
         //disp_FOTA_Printf("     Boot 2 in Normal Mode   ");
      }
      else
      {
         setup_cmdline_tag(cmdln);
         //disp_FOTA_Printf("                             ");
         //disp_FOTA_Printf("      Boot in Normal Mode    ");
      }
      setup_end_tag();

      //copy kernel to the right position
      memcpy(&KERNEL_START, &KERNEL_BUF, kernelSize);
      /*disp_FOTA_Printf("                             ");
      disp_FOTA_Printf("    Copied kernel to boot     ");
      disp_FOTA_Printf("            Wait!             ");
*/
      //SYSCON operations
      *((unsigned int*)SYSCON_NORMAL_CFG) = 0xFFFFFFFF;
      _CoDisableDCache();
      _System_DisableVIC();
      _System_DisableIRQ();
      _System_DisableFIQ();

      kernel = (fun_t)&KERNEL_START;
      kernel(0, &ATAG_MODEL, ATAG_buf);

   }
   else
   {
      disp_FOTA_Printf("                              ");
      disp_FOTA_Printf("            ERROR:            ");
      disp_FOTA_Printf(" boot.img not found or broken ");
      disp_FOTA_Printf("                              ");
      disp_FOTA_Printf("   Please re-copy boot.img    ");
      disp_FOTA_Printf("     to external SD card      ");
   }

   //loop forever
   while(1);

   return 0;
   }
}
