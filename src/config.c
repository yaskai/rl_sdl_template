#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "../include/log_message.h"
#include "../include/ansi_codes.h"

OptionTable OptionTableCreate() {
	return (OptionTable) {
		.capacity = 2,
		.count = 0,
		.entries = calloc(2, sizeof(Option))
	};
}

u16 OptionTableHash(char *key) {
	u16 hash = 5381;

	unsigned char c;
	while((c = *key++) != '\0') 
		hash = ((hash << 5)) + c;

	return hash;
}

u16 Probe(u16 id, u16 attempt, u16 size) {
	return (id + attempt) % size;
}

void OptionTableResize(OptionTable *table) {
	// Create temporary array with doubled table capacity  
	u16 new_cap = table->capacity << 1;
	Option *new_entries = calloc(new_cap, sizeof(Option)); 

	// Copy entries to their correct indices in temp array 
	for(u16 i = 0; i < table->capacity; i++) {
		Option entry = table->entries[i];
		if(!entry.key) continue; 

		u16 id = OptionTableHash(entry.key) % new_cap;
		new_entries[id] = entry;
	}

	// Set table capacity to new 
	table->capacity = new_cap;

	// Resize table array
	Option *array_ptr = realloc(table->entries, sizeof(Option) * new_cap); 
	table->entries = array_ptr;

	// Copy data, free temp array
	memcpy(table->entries, new_entries, sizeof(Option) * new_cap);
	free(new_entries);
}

void OptionTableInsert(OptionTable *table, Option entry) {
	u16 id = OptionTableHash(entry.key) % table->capacity;
	u16 attempt = 0;

	while(attempt < table->capacity) {
		if(table->entries[id].key) { 
			if(!streq(table->entries[id].key, entry.key)) {;
				id = Probe(id, ++attempt, table->capacity); 
				continue;
			}
		}

		break;
	}

	table->entries[id] = entry;		
	table->count++;
}

void ConfigInit(Config *conf) {
	for(u8 i = 0; i < OPTION_BLOCK_COUNT; i++) 
		conf->option_tables[i] = OptionTableCreate();

	Option opt_ww = (Option) {
	};
	OptionTableInsert(&conf->option_tables[OPT_BLOCK_WINDOW], opt_ww);
	
}

void ConfigClose(Config *conf) {

}

void ConfigRead(Config *conf, char *path) {
	// Open file
	FILE *pF = fopen(path, "r");	

	// Print error message and exit if file missing
	if(!pF) {
		MessageError("Could not open configuration file", path);
		return;
	}

	// Read file content by line
	char line[128];
	while(fgets(line, sizeof(line), pF)) {
		ConfigParseLine(conf, line, 1);
	}

	// Close file
	fclose(pF);
}

void ConfigParseLine(Config *conf, char *line, u8 print) {
	char *comment = strchr(line, COMMENT_MARKER);	
	if(comment) *comment = '\0';

	if(!line && !comment) return;

	if(print) {
		printf("%s", line);
		if(comment) ConfigPrintComment(comment);
	}

	char *eq = strchr(line, '=');
	if(!eq) return;

	*eq = '\0';
	char *val = line;
	char *key = eq + 1;
}

void ConfigPrintComment(char *comment) {
	comment++;

	char str_message[strlen(comment)];

	str_message[0] = '#';
	memcpy(str_message + 1, comment, strlen(comment));

	Message(str_message, ANSI_GREEN);
}

