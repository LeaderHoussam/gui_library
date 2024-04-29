//
//  ei_parser.h
//  projet_c_IG
//
//  Created by François Bérard on 10/03/16.
//  Copyright © 2016 LIG-IIHM. All rights reserved.
//

#ifndef EI_PARSER_H
#define EI_PARSER_H

#include "ei_widget.h"


int ei_parse_file(char* file_path);

ei_widget_t ei_parse_widget_from_name(char* name);

void free_name_to_widget_list(void);



#endif /* parser_h */
