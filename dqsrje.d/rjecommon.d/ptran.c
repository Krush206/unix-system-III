main()
{
	int count,i;
	char buf[512],blanks[80];

	count=0;
	for(i=0;i<80;i++)
		blanks[count]=' ';
	while(read(0,&count,1)>0){
		read(0,buf,count);
		write(1,buf,count);
		if(count<80)
			write(1,blanks,80-count);
	}
}
