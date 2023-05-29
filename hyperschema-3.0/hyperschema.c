/*
   Hyperschema.c version 3.0 is placed in the public domain by Randy Phillips (hyperschema@rebeccaphillips.com)
   because you will need to edit this code to suit your own tastes on the HTML output.  If I
   release it under the GPL that would be a real pain in the neck, as the GPL requires that you
   giving me every single one of your code changes.  If you find a bug or would like to make a
   contribution to the code, please let me know.  Bug fixes and enhancements are most welcome!
   I would like to put on the hyperschema web site a list of users, please let me know if you
   used this public domain source code.  Email me with any questions or comments, and enjoy!


   Norman Dunbar, ndunbar@lynxfinancialsystems.co.uk, has very kindly added comments
   regarding compilation of this code.  As the C code is no longer maintained in favor
   of the Python version, I add his comments here:

            In the C file, I have added a couple of lines as I run my stuff under
            Windows NT and I use Borland C++ Builder as my chosen development tool.

            at the start :

            #include <conio.h>

            and just at the end :

            printf("\n\nPress ENTER/RETURN to finish ....");
            getch();

            this allows me to see the messages/errors before NT closed the DOS box at
            the end of the program.

*/




#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <string.h>



// this structure describes a foreign key
struct fkey {
	char *table_name;
	char *column_name;
	char *data_type; // date, number, etc.
	char *data_length;
	char *data_precision;
	char *data_scale;
	char *default_length;
	char nullable;
};

// this structure describes a row of text from the schema extractor output text file
struct row {
	char *column_name;
	char *data_type; // date, number, etc.
	char *data_length;
	char *data_precision;
	char *data_scale;
	char *default_length;
	char nullable;
	char key_type; // if a primary or foreign key
	char *fkey_table_name;  // table name for foreign keys
	char *fkey_column_name; // column name for foreign keys
};

// this structure describes an individual table in the database
struct table {
	char *name;
	struct row *rows;
	unsigned long row_count;
	char *notes;
	struct fkey *fkeys; // these are foreign keys pointing to this table
	unsigned long fkey_count;
};



// This is a fairly simple program, so just add variables as needed here
struct table *table_list;
unsigned long table_count, line_count, datafile_size;
FILE *fileptr;
unsigned long *column_start_positions, *column_end_positions, column_count;
struct stat filestatus;
char *datafile_buffer, **line_list, tempchar, *tokenptr1, *tokenptr2, tempstring[1024];
unsigned long t1, t2, t3, t4, length, count, dash_line, primary_key_flag;
char *version = "3.0";



// the web pages output from this program are something you'll probably want to copyright
char *copyright_string = "Your site copyright statement, if any, goes here.";



///////////////////////////////////////////////////////////////////////////////
// This small program has linear control flow, so everything is in main()
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	
	// It's usually helpful to begin with something like this
	if (argc != 2){
		printf("Usage: hyperschema [filename]\n");
		exit(0);
	}
	
	
	
	///////////////////////////////////////
	// Simple introductory string
	///////////////////////////////////////
	printf("\nHyperschema %s - the open source\nrelational database schema hypermapper\n", version);
	

	
	///////////////////////////////////////
	// open the named file for input
	///////////////////////////////////////
	fileptr = fopen(argv[1],"rt");
	if(fileptr==NULL){
		printf("Error: unable to open %s.\n\n",argv[1]);
		exit(0);
	}
	printf("\tOpened %s for reading\n", argv[1]);
	
	
	
	///////////////////////////////////////
	// allocate space to hold file buffer.  We will read the schema extractor output
	// file into this buffer and process all data in place
	///////////////////////////////////////
	stat(argv[1], &filestatus);
	datafile_size = (unsigned long)filestatus.st_size;
	datafile_buffer = (char *)malloc((datafile_size + 1) * sizeof(char));
	if(datafile_buffer == NULL){
		printf("Unable to allocate memory for data file buffer\n\n");
		exit(0);
	}
	printf("\tAllocated main data buffer, %lu bytes (filesize + 1)\n", datafile_size + 1);
	
	
	
	///////////////////////////////////////
	// load schema extractor text file into buffer and count number of characters in the file.
	// Also count the number of carriage returns in the file to determine the number of lines
	// of text in the file
	///////////////////////////////////////
	line_count = 0;
	datafile_size = 0;
	tempchar = (char)fgetc(fileptr);
	while (tempchar != EOF){
		datafile_buffer[datafile_size]=tempchar;
		if(tempchar == '\n')
		line_count++;
		datafile_size++;
		tempchar = (char)fgetc(fileptr);
	}
	fclose(fileptr); // done with file pointer here
	printf("\tRead datafile into main buffer\n");
	
	
	
	///////////////////////////////////////
	// allocate space to hold pointers to the individual lines of text in the file
	///////////////////////////////////////
	line_list = (char **)malloc((line_count + 1) * sizeof(char *));
	if(line_list == NULL){
		printf("Unable to allocate memory for list of lines\n\n");
		exit(0);
	}
	printf("\tAllocated %lu pointers to individual lines of text\n", line_count);
	
	
	
	///////////////////////////////////////
	// assign pointers from our datafile buffer to the newly allocated line list
	// replace all carriage returns with '\0' for C string routines
	///////////////////////////////////////
	line_count = 1; // start at first line
	line_list[line_count-1] = &datafile_buffer[0];
	for(t1=1; t1<datafile_size; t1++)
	if(datafile_buffer[t1] == '\n'){ // end of line
		datafile_buffer[t1] = '\0';
		line_list[line_count] = &datafile_buffer[t1+1]; // next character is start of a line of text
		line_count++;
	}
	line_count--;
	printf("\tAssigned text line pointers and replaced \\n's with \\0's\n");
	
	
	
	///////////////////////////////////////
	// find the space-delimited line of ----'s
	// this line will allow us to analyze the datafile column text locations
	///////////////////////////////////////
	dash_line = 0;
	while(line_list[dash_line][0] != '-'){
		dash_line++;
		if(dash_line == line_count){
			printf("Could not find dash line (\"---\"'s) for column size information\n\n");
			exit(0);
		}
	}
	printf("\tData file \"dash line\" is line %lu\n", dash_line+1);
	
	
	///////////////////////////////////////
	// Now that we have found the dashed line, find the last line of data from the spooled text.
	// Spooled data file may have text extending beyond the data, but the spooled line of text after the
	// last line of data will begin with a null or a space if it exists
	///////////////////////////////////////
	t1 = dash_line;
	while ( (line_list[t1][0] != '\0') && (line_list[t1][0] != ' ') && (t1 <= line_count) ){
		t1++;
	}
	line_count = t1 - 1;
	
	
	///////////////////////////////////////
	// count number of columns in the data file by analyzing the dashed line
	///////////////////////////////////////
	column_count = 0;
	length=strlen(line_list[dash_line]);
	for(t1=1; t1<length; t1++){
		if(((line_list[dash_line][t1] == ' ') || (line_list[dash_line][t1+1] == '\0')) && (line_list[dash_line][t1-1] == '-') )
		column_count++;
	}
	printf("\tCounted %lu columns\n", column_count);
	
	
	
	///////////////////////////////////////
	// allocate memory to hold column start and end positions
	///////////////////////////////////////
	column_start_positions = (unsigned long *)malloc(column_count * sizeof(unsigned long));
	if(column_start_positions == NULL){
		printf("Unable to allocate memory for column start positions\n\n");
		exit(0);
	}
	column_end_positions = (unsigned long *)malloc(column_count * sizeof(unsigned long));
	if(column_end_positions == NULL){
		printf("Unable to allocate memory for column end positions\n\n");
		exit(0);
	}
	printf("\tAllocated memory to hold column positions\n");
	
	
	
	///////////////////////////////////////
	// store column start and end positions in newly allocated arrays
	///////////////////////////////////////
	t2 = 0;
	column_start_positions[t2] = 0;
	length = strlen(line_list[dash_line]);
	for(t1=1; t1<length; t1++){
		if(((line_list[dash_line][t1] == ' ') || (line_list[dash_line][t1+1] == '\0')) && (line_list[dash_line][t1-1] == '-') ){
			column_end_positions[t2] = t1;
			t2++;
			if(t2 < column_count) // don't assign the last one
			column_start_positions[t2] = t1+1;
		}
	}
	printf("\tDetermined column positions from dash line\n");
	
	
	
	///////////////////////////////////////
	// go through all lines of data, tokenizing all rows in place.  If no token, make an empty string.
	///////////////////////////////////////
	t1 = dash_line + 1; // start on the line of text after the ----'s
	while(t1 < line_count){ // end of data
		for(t2=0; t2<column_count; t2++){
			line_list[t1][column_end_positions[t2]] = '\0';
			tokenptr1 = strtok(&line_list[t1][column_start_positions[t2]], " ");
			if(tokenptr1 == NULL){ // replace "no tokens" with empty strings
				line_list[t1][column_start_positions[t2]] = '\0';
				} else {
				// this should be redundant but seems necessary, doesn't hurt
				strcpy(&line_list[t1][column_start_positions[t2]], tokenptr1);
			}
		}
		t1++;
	}
	printf("\tTokenized all data into individual strings\n");
	
	
	
	///////////////////////////////////////
	// count the number of tables
	///////////////////////////////////////
	table_count = 1; //  first table
	tokenptr1 = &line_list[dash_line+1][0]; // first table name is in the line of text after the ----'s
	t1 = dash_line+2; // start with next line
	while(t1 < line_count){
		if(!(strcmp(tokenptr1, &line_list[t1][0]) == 0)){ // if equal, strcmp returns zero
			table_count++;
			tokenptr1 = &line_list[t1][0];
		}
		t1++;
	}
	if(table_count < 2){
		printf("\nError: less than two tables found in datafile.\n");
		exit(0);
	}
	printf("\tCounted %lu tables\n", table_count);
	
	
	
	///////////////////////////////////////
	// allocate memory to hold table structures
	///////////////////////////////////////
	table_list = (struct table *) malloc(table_count * sizeof(struct table));
	if(table_list == NULL){
		printf("Unable to allocate memory for table list\n\n");
		exit(0);
	}
	printf("\tAllocated memory for table list\n");
	
	
	
	///////////////////////////////////////
	// Count number of rows of text from the input datafile in each table
	///////////////////////////////////////
	t2=0;
	t1 = dash_line + 1; // start on the line of text after the ----'s
	table_list[0].row_count = 0;
	while(t1 < line_count){
		if(!(strcmp(&line_list[t1][0], &line_list[t1+1][0]) == 0)){ // if equal, strcmp returns zero
			table_list[t2].row_count++; // last column in this table
			t2++; // next table
			if (t2 < table_count) // the while loop should stop after this line, so do not go to next table
			table_list[t2].row_count = 0;
			} else {
			table_list[t2].row_count++;
		}
		t1++;
	}
	printf("\tCounted the number of rows in each table\n");
	
	
	
	///////////////////////////////////////
	// allocate memory to hold datafile row data in each table structure
	///////////////////////////////////////
	for(t1=0; t1<table_count; t1++){
		table_list[t1].rows = (struct row *)malloc(table_list[t1].row_count * sizeof(struct row));
		if(table_list[t1].rows == NULL){
			printf("Unable to allocate memory for table %lu's rows\n\n", t1);
			exit(0);
		}
	}
	printf("\tAllocated memory for all rows\n");
	
	
	
	///////////////////////////////////////
	// point table structure names to elements in data file
	///////////////////////////////////////
	t3 = dash_line + 1; // start on the line of text after the -----'s
	for(t1=0; t1<table_count; t1++){
		table_list[t1].name = &line_list[t3][0]; // table names
		for(t2=0; t2<table_list[t1].row_count; t2++){
			table_list[t1].rows[t2].column_name      = &line_list[t3][column_start_positions[1]];
			table_list[t1].rows[t2].data_type        = &line_list[t3][column_start_positions[2]];
			table_list[t1].rows[t2].data_length      = &line_list[t3][column_start_positions[3]];
			table_list[t1].rows[t2].data_precision   = &line_list[t3][column_start_positions[4]];
			table_list[t1].rows[t2].data_scale       = &line_list[t3][column_start_positions[5]];
			table_list[t1].rows[t2].default_length   = &line_list[t3][column_start_positions[6]];
			table_list[t1].rows[t2].nullable         =  line_list[t3][column_start_positions[7]];
			table_list[t1].rows[t2].key_type         =  line_list[t3][column_start_positions[10]];
			table_list[t1].rows[t2].fkey_table_name  = &line_list[t3][column_start_positions[12]];
			table_list[t1].rows[t2].fkey_column_name = &line_list[t3][column_start_positions[13]];
			t3++;
		}
	}
	printf("\tAssigned each row's data pointers for all tables\n");
	
	
	
	///////////////////////////////////////
	// It is possible for a primary key to also be a foreign key.  In the data file we read,
	// this results in two rows with the same name - one a primary key, and one a foreign key.
	// Here we find these and flag them for special processing later.
	///////////////////////////////////////	
	for(t1=0; t1<table_count; t1++){
	    primary_key_flag = 0;
	    // find the primary key, if one exists in this table
	    for(t2=0; t2<table_list[t1].row_count; t2++){
		if(table_list[t1].rows[t2].key_type == 'P'){
		    primary_key_flag = 1;
		    break; // found it, t2 now contains the primary key row location
		}
	    }
	    // find another row with same column name as primary key if it exists
	    // t2 from loop above contains primary key location
	    if(primary_key_flag){
		for(t3=0; t3<table_list[t1].row_count; t3++){
		    if (t2 == t3) // don't compare primary key name with itself
			continue;
		    // strcmp returns zero if strings are exactly equal
		    if(strcmp(table_list[t1].rows[t2].column_name, table_list[t1].rows[t3].column_name) == 0){
			table_list[t1].rows[t2].key_type = 'I'; // I for "Ignore later"
			table_list[t1].rows[t3].key_type = 'B'; // B for "Both primary and foreign key"
		    }
		}
	    }
	}
	printf("\tChecked all tables for primary keys that are also foreign keys\n");



	///////////////////////////////////////
	// for each table, count the number of foreign keys pointing to this table
	///////////////////////////////////////
	for(t1=0; t1<table_count; t1++){
		table_list[t1].fkey_count = 0;
		for(t2=0; t2<table_count; t2++){
			for(t3=0; t3<table_list[t2].row_count; t3++){
				if(strcmp(table_list[t2].rows[t3].fkey_table_name, table_list[t1].name) == 0) // foreign key found
				table_list[t1].fkey_count++;
			}
		}
	}
	printf("\tCounted number of foreign keys in each table\n");
	
	
	
	///////////////////////////////////////
	// for each table, allocate memory to hold list of foreign keys structures
	///////////////////////////////////////
	for(t1=0; t1<table_count; t1++){
		if(table_list[t1].fkey_count){ // if any foreign keys, allocate memory for list
			table_list[t1].fkeys = (struct fkey *)malloc(table_list[t1].fkey_count * sizeof(struct fkey));
			if(table_list[t1].fkeys == NULL){
				printf("Unable to allocate memory for table %lu's foreign keys\n\n", t1);
				exit(0);
			}
		}
	}
	printf("\tAllocated memory for each table's foreign key list\n");
	
	
	
	///////////////////////////////////////
	// for each table, populate the list of foreign keys pointing to this table
	///////////////////////////////////////
	for(t1=0; t1<table_count; t1++){
		t4 = 0;
		if(table_list[t1].fkey_count != 0){
			for(t2=0; t2<table_count; t2++){
				for(t3=0; t3<table_list[t2].row_count; t3++){
					if(strcmp(table_list[t2].rows[t3].fkey_table_name, table_list[t1].name) == 0){ // foreign key found
						table_list[t1].fkeys[t4].table_name     = table_list[t2].name;
						table_list[t1].fkeys[t4].column_name    = table_list[t2].rows[t3].column_name;
						table_list[t1].fkeys[t4].data_type      = table_list[t2].rows[t3].data_type;
						table_list[t1].fkeys[t4].data_length    = table_list[t2].rows[t3].data_length;
						table_list[t1].fkeys[t4].data_precision = table_list[t2].rows[t3].data_precision;
						table_list[t1].fkeys[t4].data_scale     = table_list[t2].rows[t3].data_scale;
						table_list[t1].fkeys[t4].default_length = table_list[t2].rows[t3].default_length;
						table_list[t1].fkeys[t4].nullable       = table_list[t2].rows[t3].nullable;
						t4++;
					}
				}
			}
		}
	}
	printf("\tAssigned foreign key data pointers for all tables\n");
	
	
	
	///////////////////////////////////////
	// make index HTML page
	///////////////////////////////////////
	fileptr = fopen("index.html","wt");
	if(fileptr==NULL){
		printf("Error: unable to open index.html for writing\n\n");
		exit(0);
	}
	
	fprintf(fileptr, "<HTML><head><title>Hyperschema - Master Index</title></head>\n");
	fprintf(fileptr, "<BODY bgcolor=\"#ffffff\" text=\"#000000\" link=\"#0000a0\" vlink=\"#800080\" alink=\"#ff0000\">\n");
	
	// header
	fprintf(fileptr, "<A NAME=\"Top\">\n");
	fprintf(fileptr, "<CENTER><B>- Hyperschema -<BR><BR><FONT SIZE=+1>Master Index of Tables</FONT></B></CENTER><BR>\n");
	
	// Make alphabetical quick-pick headings
	fprintf(fileptr, "<CENTER>");
	fprintf(fileptr, "<A HREF=\"#quick_%c\">%c</A>", table_list[0].name[0], table_list[0].name[0]);
	for(t1=1; t1<table_count; t1++)
	if(table_list[t1].name[0] != table_list[t1-1].name[0])
	fprintf(fileptr, " &nbsp;&nbsp;&nbsp;<A HREF=\"#quick_%c\">%c</A>", table_list[t1].name[0], table_list[t1].name[0]);
	fprintf(fileptr, "</CENTER><BR>\n");
	
	// populate index, adding internal bookmarks for quick_picks at top of list
	fprintf(fileptr, "<TABLE ALIGN=\"CENTER\" cellpadding=\"3\">\n");
	fprintf(fileptr, "<TR><TD><BR><BR><CENTER>");
	fprintf(fileptr, "<A NAME=\"quick_%c\"><A HREF=\"#Top\">Back to Top</A></CENTER><BR><BR><A HREF=\"%s.html\">%s</A></TD></TR>\n", table_list[0].name[0], table_list[0].name, table_list[0].name);
	for(t1=1; t1<table_count; t1++){
		if(table_list[t1].name[0] != table_list[t1-1].name[0]){ // new quick-pick found, add bookmark
			fprintf(fileptr, "<TR><TD><BR><BR><CENTER>");
			fprintf(fileptr, "<A NAME=\"quick_%c\"><A HREF=\"#Top\">Back to Top</A></CENTER><BR><BR><A HREF=\"%s.html\">%s</A></TD></TR>\n", table_list[t1].name[0], table_list[t1].name, table_list[t1].name);
		} else
		fprintf(fileptr, "<TR><TD><A HREF=\"%s.html\">%s</A></TD></TR>\n", table_list[t1].name, table_list[t1].name);
	}
	fprintf(fileptr, "</TABLE><BR>");
	
	
	// footer
	fprintf(fileptr, "<BR><BR><CENTER><FONT SIZE=-1>\n");
	fprintf(fileptr, "Hyperschema version %s is brought to you by <A HREF='http://www.epenergy.com'>El Paso Energy</A>'s<BR>\n", version);
	fprintf(fileptr, "<IMG SRC=\"prodsup.gif\"><BR><BR>\n");
	fprintf(fileptr, "%s\n", copyright_string);
	fprintf(fileptr, "</FONT></CENTER>\n");
	
	// padding the bottom of the pages with returns helps the bottom tables look nice
	for(t3=0; t3<35; t3++)
	fprintf(fileptr, "<BR>");
	fprintf(fileptr, "\n");
	
	fprintf(fileptr, "</BODY></HTML>\n");
	fclose(fileptr);
	printf("\tCompleted master index page\n");
	
	
	
	///////////////////////////////////////
	// make individual HTML pages
	///////////////////////////////////////
	for(t1=0; t1<table_count; t1++){
		sprintf(tempstring, "%s.html", table_list[t1].name);
		fileptr = fopen(tempstring, "wt");
		if(fileptr==NULL){
			printf("Error: unable to open %s for writing\n\n", tempstring);
			exit(0);
		}
		
		fprintf(fileptr, "<HTML><head><title>Hyperschema - %s</title></head>\n", table_list[t1].name);
		fprintf(fileptr, "<BODY bgcolor=\"#ffffff\" text=\"#000000\" link=\"#0000a0\" vlink=\"#800080\" alink=\"#ff0000\">\n");
		fprintf(fileptr, "<div id=\"overDiv\" style=\"position:absolute; visibility:hide;z-index:1;\"></div>\n");
		fprintf(fileptr, "<SCRIPT LANGUAGE=\"JavaScript\" SRC=\"overlib.js\"></SCRIPT>\n");
		
		// header
		fprintf(fileptr, "<A NAME=\"Top\">\n");
		fprintf(fileptr, "<A HREF=\"index.html\">Master Index</A>\n");  // top left corner
		fprintf(fileptr, "<CENTER><B><FONT SIZE=+2>%s</FONT></B></CENTER><BR>\n", table_list[t1].name);
		
		
		if(table_list[t1].fkey_count == 0){
			fprintf(fileptr, "<CENTER>No foreign keys point to this table.</CENTER><BR>\n");
			} else {
			fprintf(fileptr, "<CENTER><A HREF=\"#fkeys\">Foreign keys pointing to this table:</A></CENTER><BR>\n");
		}
		
		fprintf(fileptr, "<TABLE ALIGN=\"CENTER\" BORDER=\"1\" cellpadding=\"3\">\n");
		fprintf(fileptr, "<TR BGCOLOR=\"#D0D0D0\"><TH>Column Name</TH><TH>Data Type</TH><TH>Nullable</TH></TR>\n");
		
		for(t2=0; t2<table_list[t1].row_count; t2++){			
		    if(table_list[t1].rows[t2].key_type != 'I') { // if table is not ignored above
			fprintf(fileptr, "<TR><TD>"); // column in HTML
			
			// key type: primary, foreign, or ordinary
			if(table_list[t1].rows[t2].key_type == 'P') // primary key, font size +1 and bold
			    fprintf(fileptr, "<FONT SIZE=+1><B>%s </B></FONT><FONT SIZE=-1>(primary key)</FONT>\n", table_list[t1].rows[t2].column_name);
			else if(table_list[t1].rows[t2].key_type == 'R') // foreign key, make javascript output for mouseover
			    fprintf(fileptr, "<A HREF=\"%s.html\" onMouseOver=\"overlib('This foreign key is <B>%s.%s</B>')\" onMouseOut=\"nd()\">%s</A>\n",
				    table_list[t1].rows[t2].fkey_table_name,
				    table_list[t1].rows[t2].fkey_table_name,
				    table_list[t1].rows[t2].fkey_column_name,
				    table_list[t1].rows[t2].column_name);
			else if(table_list[t1].rows[t2].key_type == 'B') // Both a primary and foreign key, make javascript output for mouseover
			    fprintf(fileptr, "<A HREF=\"%s.html\" onMouseOver=\"overlib('This primary key is <B>%s.%s</B>')\" onMouseOut=\"nd()\"><FONT SIZE=+1><B>%s </B></FONT><FONT SIZE=-1>(primary key)</FONT></A>\n",
				    table_list[t1].rows[t2].fkey_table_name,
				    table_list[t1].rows[t2].fkey_table_name,
				    table_list[t1].rows[t2].fkey_column_name,
				    table_list[t1].rows[t2].column_name);
			else
			    fprintf(fileptr, "%s\n", table_list[t1].rows[t2].column_name);
			
			// data type
			fprintf(fileptr, "</TD><TD>%s", table_list[t1].rows[t2].data_type);
			if(strcmp(table_list[t1].rows[t2].data_type, "NUMBER") == 0)
			    if(*table_list[t1].rows[t2].data_precision != '\0') // not all numbers have precision specifications
				fprintf(fileptr, "&nbsp;&nbsp;(%s,%s)", table_list[t1].rows[t2].data_precision, table_list[t1].rows[t2].data_scale);
			if(strcmp(table_list[t1].rows[t2].data_type, "VARCHAR2") == 0)
			    fprintf(fileptr, "&nbsp;&nbsp;[%s]", table_list[t1].rows[t2].data_length);
			fprintf(fileptr, "\n");
			
			if(table_list[t1].rows[t2].nullable == 'N')
			    fprintf(fileptr, "</TD><TD><CENTER>No</CENTER></TD></TR>\n");
			else
			    fprintf(fileptr, "</TD><TD><CENTER>Yes</CENTER></TD></TR>\n");
		    }
		}
		
		fprintf(fileptr, "</TABLE><BR>\n");
		fprintf(fileptr, "<BR><BR>\n");
		
		if(table_list[t1].fkey_count != 0){
			fprintf(fileptr, "<A NAME=\"fkeys\"></A><A HREF=\"#Top\">Back to top</A><CENTER>Foreign keys pointing to <B>%s</B>:</CENTER><BR>\n", table_list[t1].name);
			
			fprintf(fileptr, "<TABLE ALIGN=\"CENTER\" BORDER=\"1\" cellpadding=\"3\">\n");
			fprintf(fileptr, "<TR BGCOLOR=\"#D0D0D0\"><TH>Table Name</TH><TH>Column Name</TH></TR>\n");
			
			for(t2=0; t2<table_list[t1].fkey_count; t2++){
				fprintf(fileptr, "<TR><TD><A HREF=\"%s.html\">%s</A></TD><TD>%s</TD></TR>",
					table_list[t1].fkeys[t2].table_name,
					table_list[t1].fkeys[t2].table_name,
				table_list[t1].fkeys[t2].column_name);
			}
			
			fprintf(fileptr, "</TABLE><BR>\n");
		}
		
		// footer
		fprintf(fileptr, "<BR><BR><CENTER><FONT SIZE=-1>\n");
		fprintf(fileptr, "Hyperschema version %s is brought to you by <A HREF='http://www.epenergy.com'>El Paso Energy</A>'s<BR>\n", version);
		fprintf(fileptr, "<IMG SRC=\"prodsup.gif\"><BR><BR>\n");
		fprintf(fileptr, "%s\n", copyright_string);
		fprintf(fileptr, "</FONT></CENTER>\n");
		
		// padding the bottom of the pages with returns helps the foreign key table reference look nice
		for(t3=0; t3<35; t3++)
		fprintf(fileptr, "<BR>");
		fprintf(fileptr, "\n");
		
		fprintf(fileptr, "</BODY></HTML>");
		fclose(fileptr);
	}
	
	printf("\tProcessed %lu HTML pages\nFinished!\n", table_count);
	
	
	
	///////////////////////////////////////
	// We're done!
	///////////////////////////////////////
	return(0);
	
}
