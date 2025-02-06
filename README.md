mytoolkit.x
		
	-PLEASE NOTE: Redirecting a file for input during execution causes infinite loop
	
	-Commands must be typed individually, I am unsure why this is the case, maybe due to lack of control d signal handling	

	-DOES NOT HAVE THE CONTROL D SIGNAL HANDLING WORKING
	
	-All executables work with this

	-Executables such as a.out or one called test MUST need ./a.out or ./test in order to function properly but they work as intended! 

	-Single piping is it's own case and then multiple pipes are able to be supported after. I would max test 4 but it is capable of more

	-The following works for input/output redirection:  command1 < test1.txt     command1 > test1.txt   command1 < test1.txt > test2.txt
		No other i/0 style redirection was meant to be implemented and this is what we have

	- Please Make Sure all commands are seperated by space  such as: command1 -args   or in cases of piping: ls | sort | wc or I/0 redirect: command1 > test.txt

	-I decided to use fork/execvp for this program because I deemed it rather practical to simply create the new processes and run them inside my program rather than creating brand new external ones. I do not have the most experience with forking/execvp and this assignment was a super useful workshop in figuring out how to use them. That is is why i chose to do my special commands: mycd, mypwd and myexit this way.



