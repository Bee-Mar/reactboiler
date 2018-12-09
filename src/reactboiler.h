#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// error messages
#define JSX_ERROR_MSG "\n\tERROR: --jsx must be used with --styled/--class args"
#define NO_FILE_ERROR_MSG "\n\tERROR: No file/directory/class name provided"
#define NO_CL_ARGS_ERROR_MSG "\n\tERROR: No command line options provided"
#define INVALID_ARGS_ERROR_MSG "\n\tERROR: Invalid arguments\n"

// imports/exports
#define IMPORT_REACT_COMPONENT "import React, { Component } from 'react';\n"
#define IMPORT_CONNECT "import { connect } from 'react-redux';\n"
#define IMPORT_STYLED "import styled from 'styled-components';\n"
#define EXPORT_ACTION "export const ACTION = 'ACTION';\n\n"
#define IMPORT_STYLED_WITHTHEME                                                \
  "import styled, { withTheme } from 'styled-components';\n"

// flags
#define DIR 0
#define REDUX 1
#define WITH_THEME 2
#define STYLED 3
#define CLASS 4
#define JSX 5
#define ANY_FLAG 6

// Redux types
#define ACTIONS "Actions"
#define ACTION_TYPES "ActionTypes"
#define REDUCERS "Reducers"

// file extensions
#define ACTIONS_JS "Actions.js"
#define ACTION_TYPES_JS "ActionTypes.js"
#define REDUCERS_JS "Reducers.js"

#define DIV_TAGS "\n      <div>\n      </div>\n    "

void error_msg(char *msg);

void verify_cl_args(int argc, char *argv[]);

void execute_options(char *argv[]);
