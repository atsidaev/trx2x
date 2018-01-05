#include <iostream>
using namespace std;

#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "DiskImage.h"
#include "utils.h"

int main(int argc, char *argv[])
{
   cout << "TRX2X converter V1.2 beta 2    (C)2002 Alex Makeev" << endl;
   cout << endl;
   if(argc!=3)
   {
      cout << "Input:  UDI, TRD, SCL, FDI, TD0, FDD, hobeta (.$*|.!*) " << endl;
      cout << "Output: UDI, TRD, SCL, FDI, TD0, FDD " << endl;
      cout << endl << "USAGE: TRX2X.EXE <filename.trd> <filename.udi> " << endl;
      cout << endl << "Based on UltraDiskImage engine by Alex Makeev" << endl;
      cout << "WWW: http://zxmak.chat.ru/" << endl;
      return 1;
   }

   char infilename[4096];
   char outfilename[4096];

   if( !strcmp(argv[1], argv[2]) )
   {
      cout << "Error: file self overwrite is prohibited!" << endl;
      return 2;
   }
   cout << argv[1] << " ---> " << argv[2] << "..." << endl << endl;

   if(!file_exists(argv[1])) // file not exists!!!
   {
      cout << "Error: source file not found!" << endl;
      return 3;
   }

   if(file_exists(argv[2])) // file exists!!!
   {
      cout << "Warning: destination file already exists!" << endl;
      cout << "Are you sure to overwrite? (y/n) ";
      cout.flush();
      int cr = getchar();
      cout << endl;
      if( (cr!='y')&&(cr!='Y') ) 
      {
         cout << "Operation aborted!" << endl;
         return 4;
      }
   }

// detect output type:
   TDiskImageType typ = DIT_UNK;

   char ext[MAX_EXT];
   const char* ext_pos = get_extension(argv[2]);
   if (ext_pos) {
      strncpy(ext, ext_pos, MAX_EXT);
      make_uppercase(ext);
      if(!strcmp(ext, ".TRD")) typ = DIT_TRD;
      if(!strcmp(ext, ".SCL")) typ = DIT_SCL;
      if(!strcmp(ext, ".FDI")) typ = DIT_FDI;
      if(!strcmp(ext, ".UDI")) typ = DIT_UDI;
      if(!strcmp(ext, ".TD0")) typ = DIT_TD0;
      if(!strcmp(ext, ".FDD")) typ = DIT_FDD;
   }

   if(typ == DIT_UNK)
   {
      cout << "Error: unsupported output format '" << ext << "'!" << endl;
      return 5;
   }

   if(typ != DIT_UDI) cout << "warning: destination file is not UDI! Some information may be loss!" << endl << endl;

   TDiskImage *img1 = new TDiskImage;
   img1->AddBOOT = false;
   img1->Open(argv[1], true);
   if(!img1->DiskPresent)
   {
      cout << "Error: load source file failed!" << endl;
      return 6;
   }
   cout << "source file successfully loaded..." << endl;

   int hf = open(argv[2], O_CREAT | O_RDWR | O_TRUNC | O_BINARY, 0666);
   if(hf < 0)
   {
      delete img1;
      perror("Create file");
      return 7;
   }

   if(typ==DIT_TRD) img1->writeTRD(hf);
   if(typ==DIT_SCL) img1->writeSCL(hf);
   if(typ==DIT_FDI) img1->writeFDI(hf);
   if(typ==DIT_UDI) img1->writeUDI(hf);
   if(typ==DIT_TD0) img1->writeTD0(hf);
   if(typ==DIT_FDD) img1->writeFDD(hf);

   close(hf);

   cout << "destination file successfully saved..." << endl;

   delete img1;
   return 0;
}
