myftpd: server.o commands.o connection.o stream.o logging.o
	gcc server.o commands.o connection.o stream.o logging.o -o myftpd

server.o: server.c commands.h connection.h stream.h logging.h
	gcc -Wall -c server.c

connection.o: connection.c connection.h
	gcc -Wall -c connection.c

commands.o: commands.c stream.h logging.h
	gcc -Wall -c commands.c
			
stream.o: stream.c stream.h
	gcc -Wall -c stream.c

logging.o: logging.c logging.h
	gcc -Wall -c logging.c
	
clean:
	rm *.o



