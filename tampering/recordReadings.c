#include "/home/belal-a-aziz/Desktop/RecordInFile/recordReadings .h" 

 void writeFile(char * FileName , char * DataString)
 {
 /*	int fd;
 	filePointer = fopen(FileName,"a");	// W -> For create text file ,, a -> For upadte in exciting file
       fprintf(filePointer,"%s",DataString);
*/     

      	filePointer = fopen(FileName,"a");	// W -> For create text file ,, a -> For upadte in exciting file
	  struct flock lock;
	  lock.l_type = F_WRLCK;    // read/write (exclusive versus shared) lock 
	  lock.l_whence = SEEK_SET; // base for seek offsets 
	  lock.l_start = 0;         // 1st byte in file 
	  lock.l_len = 0;           // 0 here means 'until EOF' 
	  lock.l_pid = getpid();    // process id 
	int fd;	
	// file descriptor to identify a file within a process 
	  if ((fd = open(FileName, O_RDWR | O_CREAT, 0777)) < 0)  // -1 signals an error 
	    printf("open failed...");
	  if (fcntl(fd, F_SETLK, &lock) < 0) // F_SETLK doesn't block, F_SETLKW does 
	    printf("fcntl failed to get lock...");
	  else 
	  {
	    //write(fd, DataString, strlen(DataString)); // populate data file 
	     fprintf(filePointer,"%s",DataString);
	//    fprintf(stderr, "Process %d has written to data file...\n", lock.l_pid);
	  }

 	// Now release the lock explicitly. 
	  lock.l_type = F_UNLCK;
	  if (fcntl(fd, F_SETLK, &lock) < 0)
	  printf("explicit unlocking failed..."); 
	  close(fd); // close the file: would unlock if needed 
	fclose(filePointer);
  }

/*
int main()
{
	char buf[120]="hello mother Fucker\n";	
	for(int i=0;i<1024;i++)
	{
		writeFile(PATH,buf);
		printf("%s",buf);
	}	
	return 0;	
}
*/
