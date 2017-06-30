/**
 *
 * Author	: undefined
 * Version	: 1.0
 * Name		: blocker
 * Description	: blocks websites and application specified in 
 *			the configuration file. 
 * 
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef HOSTS_FILE
	#define HOSTS_FILE "/etc/hosts" 
#endif
#ifndef CONFIG_FILE
	#define CONFIG_FILE ".blockrc" 
#endif

/*
 * [CONFIG_TAG] its the msg which will de displayed as comment
 * in the [HOSTS_FILE] file,This will be used to separate our
 * added lines(i.e added line by this tool), with other line.
 *
 * Warning : As [CONFIG_TAG] is used to distinguish our customly
 * added line in [HOSTS_FILE] with other lines. It should strickly be an UNIQ STRING
 * on it own in the [HOSTS_FILE].
 * (string like "  " "\n","\t ", "127.0.0.1") will lead to an catastrophe.
 *
 * so "FKING DONT TRY, AND THEN ASK ME Y".
 *
 */
#ifndef CONFIG_TAG
	#define CONFIG_TAG "BLOCKER'S SPACE"
#endif

/*
 * [WEB_TAG] and [APP_TAG] are String used to devide the [CONFIG_FILE] into two parts.
 * 
 * [WEB_TAG] part of the file containing the urls of the website to block and,
 * [APP_TAG] part of the file containing the urls of paths_to_application to block.
 *
 */
#define WEB_TAG "URLS"
#define APP_TAG "PATHS"

void unblock_websites(void);
int block_website(FILE *hosts_fp, char *url);

void block_app(char *path);
void unblock_apps(void);

void help(char *tool_name);

int main(int argc, char **argv)
{	

	int is_url = 0, is_path = 0;

	char buff[255];
	FILE *hosts_fp, *config_fp;
	
	/*
	 * Options which can be used by the user.
	 */
	if(argc == 1){

		help(argv[0]);
		exit(1);
	
	} if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "?") == 0) {

		help(argv[0]);
		exit(1);

	} else if (strcmp(argv[1], "-block") == 0) {
		/*
		 * As the [HOSTS_FILE] can contain older lines written by us
		 * we will remove it and change with new lines (configurations),
		 * depending uppon our new [CONFIG_FILE] file.
		 *
		 * After removing our old changes we will we open and re-write 
		 * new ones.
		 *
		 */
		unblock_websites();

		hosts_fp = fopen(HOSTS_FILE, "a");
		config_fp = fopen(CONFIG_FILE, "r");

	} else if (strcmp(argv[1], "-unblock") == 0) {
		/*
		 * Reverts back changes.
		 * (i.e unblocks applications and websites )
		 *
		 */
		unblock_websites();
		unblock_apps();
		exit(1);
	
	} else if (strcmp(argv[1], "-edit") == 0) {
		/*
		 * Open the default editor with file_name [CONFIG_FILE],
		 * to add,edit changes in the [CONFIG_FILE]
		 *
		 */
		char cmd[300];
		sprintf(cmd, "edit %s", CONFIG_FILE);
		system(cmd);
		exit(1);

	} else {
		/*
		 * Incase of invailed options enter by user.
		 */
		fprintf(stderr, "%s : [%s] no such option found, use option [-help] for help\n", argv[0], argv[1]);
		exit(-1);
	}


	if (hosts_fp == NULL || config_fp == NULL) {
		
		fprintf(stderr, "%s : Not able to open [%s]\n", argv[0], HOSTS_FILE);
		exit(-1);

	}
	
	/*
	 * Writing the [CONFIG_TAG] separator to the [HOSTS_FILE],
	 * "ith some desing and stuff to make it look pretty".
	 * 
	 */	
	fprintf(hosts_fp, "#======== %s ========\n", CONFIG_TAG);
	fprintf(hosts_fp, "#[ %s ]\n\n","BLACK_LISTED_WEBSTES");
	
	/*
	 * We scan the [CONFIG_FILE] line by line
	 * The [CONFIG_FILE] contains of two parts (urls/paths_to_applications)
	 * differentiated by strings [WEB_TAG] && [APP_TAG]
	 * will use this TAGS to to check in which section are we.
	 * the 
	 *	is_url ,is_path 
	 * are used as flag and set to '1' when true.
	 *	(i.e) if we are in the [WEB_TAG] part.
	 *	is_url = 1;
	 *	and if in [APP_TAG]
	 *	is_path = 1;
	 * and vise versa.
	 *
	 * using this flag we will use the appropriate functions of
	 *	block_website(); 
	 *	block_app();
	 * on url/path in the [CONFIG_FILE] scanned.
	 *
	 */
	while (fgets(buff, 255, config_fp) != NULL) {
	
		if (strstr(buff, WEB_TAG) != NULL && strstr(buff, "#") == NULL) {
			is_url = 1;
			is_path = 0;
			continue;
		} else if (strstr(buff, APP_TAG) != NULL && strstr(buff, "#") == NULL) {
			is_url = 0;
			is_path = 1;
			continue;
		} else if (strstr(buff, " ") !=NULL || strcmp(buff, "\n") == 0){
			continue;
		}	
		
	
		if (is_url == 1)
			block_website(hosts_fp, buff);
		else if (is_path == 1)
			block_app(buff);

	}
	
	fprintf(hosts_fp, "\n\n#===================================================\n");
	fprintf(hosts_fp, "#!! WARNING : NATHING SHOULD BE ENTERED BELOW THIS!! \n");
	fprintf(hosts_fp, "#===================================================\n");

	fclose(hosts_fp);
	fclose(config_fp);
	return 0;
}

/*
 * void help(char *tool_name)
 *	Displays information about the help option available to the user.
 *
 */
void help(char *tool_name)
{
	printf("%s : -[help | block | unblock | edit]\n\n", tool_name);
	printf("%20s : Used to show help options available with the tools.\n", "-help");
	printf("%20s : Used to block all the urls/application specified in the configuration file\n", "-block");
	printf("%20s : Used to Unblock the urls/application specified in the configuration file\n", "-unblock");
	printf("%20s : Opens the configuration file in the editor(default.)\n", "-edit" );
}


/*
 * int block_website(FILE *hosts_fp, char *url)
 *	Blocks website by putting them into the [HOSTS_FILE] file.
 *
 * arguments :
 *	 pointer to hosts_file, url of the website to block.
 *
 * return : 
 *	return 0 on success and -1 on failure.
 */
int block_website(FILE *hosts_fp, char *url)
{	
	if(hosts_fp == NULL) return -1;

	fprintf(hosts_fp, "127.0.0.1\t%s", url);
	
	return 0;	
}

/* 
 * void unblock_websites(void)
 *	Unblocks all the websites specified in the [CONF_FILE] file.
 *	it an independend function, so it doent need an external FILE pointer to 
 *	be specified for it.
 *
 *	Its done by calculating (summing) the length of the line's in [HOSTS_FILE]
 *	till we reach the line with [CONFIG_TAG] string , and then truncating the 
 *	file with new_length ,thus part of the file from [CONFIG_TAG] and below 
 *	is deleted.  
 *
 */
void unblock_websites(void)
{	
	unsigned long new_length;
	char buff[255];

	char config_tab[255] = CONFIG_TAG;

	FILE *hosts_fp = fopen(HOSTS_FILE, "a+");
	
	if(hosts_fp == NULL) return;
	
	new_length=0;
	while(fgets(buff, 255, (FILE*)hosts_fp) !=NULL) {
	
		if(strstr(buff, config_tab) != NULL) {
			truncate(HOSTS_FILE, new_length);
			return;
		}else {
		
			new_length+=strlen(buff);
		}
	}

	fclose(hosts_fp);
}

/*
 * void block_app(char *path)
 *	Its blocks (i.e remove the execution bit for  user|group|other from the file)
 * argument:
 *	path_to_application to block.
 */
void block_app(char *path)
{	
	char cmd[300];
	sprintf(cmd, "chmod ugo-x  %s", path);
	system(cmd);
}

/*
 * void unblock_apps(void)
 *	It unblocks (i.e adds the execution bit for the user|group to the file)
 *
 *	Its scan the [APP_TAG] part of the [CONFIG_FILE] file and unblocks all the
 *	path_to_applications(paths) given in it.
 *
 */
void unblock_apps(void)
{
	FILE *config_fp = fopen(CONFIG_FILE, "r");
	
	if (config_fp == NULL) return;

	char buff[255];
	int is_path=0;
	while (fgets(buff, 255, config_fp) != NULL) {
		if (strstr(buff, WEB_TAG) != NULL && strstr(buff, "#") == NULL) {
			is_path = 0;
			continue;
		} else if (strstr(buff, APP_TAG) != NULL) {
			is_path = 1;
			continue;
		} else if (strstr(buff, " ") !=NULL || strcmp(buff, "\n") == 0){
			continue;
		}	
		
		if (is_path == 1) {
			
			char cmd[300];
			sprintf(cmd, "chmod ug+x %s", buff);
			system(cmd);

		}

	}

	fclose(config_fp);
}
