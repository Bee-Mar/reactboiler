#include "reactboiler.h"

int i, index_ctr = 0, name_indexes[BUFSIZ / 4], flag[] = {0, 0, 0, 0, 0, 0, 0};

char working_dir[BUFSIZ / 4][128], curr_dir[BUFSIZ / 4][128],
    comp_file[BUFSIZ / 4][128];

FILE *comp_fp[128], *redux_a_fp[128], *redux_at_fp[128], *redux_r_fp[128];

size_t input_sz[BUFSIZ / 4];

void error_msg(char *msg) {
  printf("%s\n\tSee man page for help.\n\n", msg);
  exit(0);
}

void verify_cl_args(int argc, char *argv[]) {

  if (argc < 2)
    error_msg(NO_CL_ARGS_ERROR_MSG);

  int i;
  char file_ext[5] = ".js\0";
  char jsx_file_ext[5] = ".jsx\0";

  for (i = 1; i < argc; i++) {

    if (strcmp(argv[i], "--dir") == 0)
      flag[DIR] = flag[ANY_FLAG] = 1;

    else if (strcmp(argv[i], "--styled") == 0)
      flag[STYLED] = flag[ANY_FLAG] = 1;

    else if (strcmp(argv[i], "--class") == 0)
      flag[CLASS] = flag[ANY_FLAG] = 1;

    else if (strcmp(argv[i], "--redux") == 0)
      flag[REDUX] = flag[ANY_FLAG] = 1;

    else if (strcmp(argv[i], "--withtheme") == 0)
      flag[WITH_THEME] = flag[ANY_FLAG] = 1;

    else if (strcmp(argv[i], "--jsx") == 0) {
      flag[JSX] = flag[ANY_FLAG] = 1;
      memcpy(file_ext, jsx_file_ext, sizeof(jsx_file_ext));
    }

    else if (argv[i][0] == '-' || (argv[i][0] == '-' && argv[i][1] == '-') ||
             strcmp(argv[i], "Dir") == 0 || strcmp(argv[i], "styled") == 0 ||
             strcmp(argv[i], "class") == 0 || strcmp(argv[i], "redux") == 0 ||
             strcmp(argv[i], "jsx") == 0 || strcmp(argv[i], "withtheme") == 0) {

      error_msg(INVALID_ARGS_ERROR_MSG);
    }

    else
      name_indexes[index_ctr++] = i;
  }

  if (!flag[ANY_FLAG])
    error_msg(NO_CL_ARGS_ERROR_MSG);

  else if (flag[JSX] && !flag[CLASS] && !flag[STYLED])
    error_msg(JSX_ERROR_MSG);

  else if (!index_ctr)
    error_msg(NO_FILE_ERROR_MSG);

  for (i = 0; i < index_ctr; i++) {
    input_sz[i] = strlen(argv[name_indexes[i]]);

    if (getcwd(working_dir[i], sizeof(working_dir[i]) + 1) != NULL)
      strcat(working_dir[i], (const char *)"/\0");

    strncpy(comp_file[i], argv[name_indexes[i]], input_sz[i]);
    strcat(comp_file[i], (const char *)file_ext);
  }
}

void execute_options(char *argv[]) {
  struct stat status = {0};

  if (flag[DIR]) {

    for (i = 0; i < index_ctr; i++) {
      strcat(working_dir[i], argv[name_indexes[i]]);
      strcat(working_dir[i], (const char *)"/\0");

      if (stat(working_dir[i], &status) == -1)
        mkdir(working_dir[i], 0755);
    }
  }

  if (flag[CLASS]) {
    char lower[BUFSIZ / 4][128];
    for (i = 0; i < index_ctr; i++) {
      strncpy(curr_dir[i], working_dir[i], sizeof(working_dir[i]));
      comp_fp[i] = fopen(strcat(curr_dir[i], comp_file[i]), "a+");
      fprintf(comp_fp[i], "%s", IMPORT_REACT_COMPONENT);

      if (flag[REDUX]) {
        strncpy(lower[i], argv[name_indexes[i]], input_sz[i]);
        lower[i][0] = (char)((int)argv[name_indexes[i]][0] + 32);
        strcat(lower[i], ACTIONS);
        fprintf(comp_fp[i],
                "%s"
                "import { actionName } from './%s';\n",
                IMPORT_CONNECT, lower[i]);
      }

      if (flag[STYLED]) {
        fprintf(comp_fp[i],
                "%s"
                "\nconst %sContainer = styled.div`\n  width: 100%%;"
                "\n  height: 100%%;\n`;\n"
                "\nclass %s extends Component{\n\n"
                "  render() {\n    return(\n      <%sContainer>"
                "\n      </%sContainer>\n    );\n  }\n}",
                flag[WITH_THEME] ? IMPORT_STYLED_WITHTHEME : IMPORT_STYLED,
                argv[name_indexes[i]], argv[name_indexes[i]],
                argv[name_indexes[i]], argv[name_indexes[i]]);
      }

      else
        fprintf(comp_fp[i],
                "\nclass %s extends Component{\n\n"
                "  render() {\n    return(%s);\n  }\n}",
                argv[name_indexes[i]], DIV_TAGS);

      if (!flag[REDUX])
        fprintf(comp_fp[i], "\n\nexport default %s;\n", argv[name_indexes[i]]);

      else {
        char lower[BUFSIZ / 4][128];
        char temp_dir[BUFSIZ / 4][128];
        strncpy(temp_dir[i], working_dir[i], sizeof(working_dir[i]));
        strncpy(lower[i], argv[name_indexes[i]], input_sz[i]);
        lower[i][0] = tolower(lower[i][0]);
        fprintf(comp_fp[i],
                "\n\nconst mapStateToProps = state => ({"
                "\n  %s: state.%s,\n});"
                "\n\nconst mapDispatchToProps = dispatch => ({\n"
                "   actionName: data => {\n    dispatch(actionName(data));"
                "\n  },\n});\n\nexport default connect(\n  mapStateToProps,"
                "\n  mapDispatchToProps,\n)("
                "%s%s%s);\n",
                lower[i], lower[i], flag[WITH_THEME] ? "withTheme(" : "",
                argv[name_indexes[i]], flag[WITH_THEME] ? ")" : "");
      }

      fclose(comp_fp[i]);
    }
  }

  if (flag[STYLED] && !flag[CLASS]) {

    for (i = 0; i < index_ctr; i++) {
      char temp_dir[sizeof(working_dir[i])];
      strncpy(temp_dir, working_dir[i], sizeof(working_dir[i]));
      comp_fp[i] = fopen(strcat(temp_dir, comp_file[i]), "a+");

      fprintf(comp_fp[i],
              "%s"
              "\nexport const %sComponent = styled.div`\n  width: 100%%;"
              "\n  height: 100%%;\n`;\n\n",
              IMPORT_STYLED, argv[name_indexes[i]]);
    }
  }

  if (flag[REDUX]) {

    char lower[BUFSIZ / 4][128];

    char actions_js[BUFSIZ / 4][128];
    char action_types_js[BUFSIZ / 4][128];
    char reducers_js[BUFSIZ / 4][128];
    char action_types[BUFSIZ / 4][128];

    char temp_dir[BUFSIZ / 4][128];

    for (i = 0; i < index_ctr; i++) {
      strncpy(temp_dir[i], working_dir[i], sizeof(working_dir[i]));

      strncpy(lower[i], argv[name_indexes[i]], input_sz[i]);
      lower[i][0] = (char)((int)argv[name_indexes[i]][0] + 32);

      strncpy(action_types[i], lower[i], sizeof(lower[i]));
      strcat(action_types[i], ACTION_TYPES);

      strncpy(actions_js[i], lower[i], sizeof(lower[i]));
      strncpy(action_types_js[i], lower[i], sizeof(lower[i]));
      strncpy(reducers_js[i], lower[i], sizeof(lower[i]));

      strcat(actions_js[i], ACTIONS_JS);
      strcat(action_types_js[i], ACTION_TYPES_JS);
      strcat(reducers_js[i], REDUCERS_JS);

      redux_a_fp[i] = fopen(strcat(temp_dir[i], actions_js[i]), "a+");
      memset(temp_dir[i], 0, sizeof(temp_dir[i]));
      strncpy(temp_dir[i], working_dir[i], sizeof(working_dir[i]));

      redux_at_fp[i] = fopen(strcat(temp_dir[i], action_types_js[i]), "a+");
      memset(temp_dir[i], 0, sizeof(temp_dir[i]));
      strncpy(temp_dir[i], working_dir[i], sizeof(temp_dir[i]));

      redux_r_fp[i] = fopen(strcat(temp_dir[i], reducers_js[i]), "a+");
      memset(temp_dir[i], 0, sizeof(temp_dir[i]));

      fprintf(redux_at_fp[i], EXPORT_ACTION);

      fprintf(redux_a_fp[i],
              "import { ACTION } from './%s';\n\n"
              "export function actionName(data) {\n"
              "  return {\n    type: ACTION,\n    payload: {\n"
              "      data: data\n    }\n  }\n}\n\n",
              action_types[i]);

      fprintf(redux_r_fp[i],
              "import { ACTION } from './%s';\n\n"
              "const initialState = {};\n\n"
              "export const %sReducer = function(state = initialState,"
              " action) {\n\n  switch(action.type) {\n\n    default:"
              "\n      return state;\n  }\n}\n\n",
              action_types[i], lower[i]);

      fclose(redux_a_fp[i]);
      fclose(redux_at_fp[i]);
      fclose(redux_r_fp[i]);
    }
  }
}
