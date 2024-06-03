#include "s21_grep.h"

void solut(FILE *file, int *flags, Regex *head, char *namefile, int i, int argc,
           char *lch);
int same(int invers, int result, int *ff);
int vflag(int vflag);
void cflag(int flag, int *ppr, int cflag, int *c);
int lflag(int ff, int lflag, char *namefile);
void nflag(int *pn);
int hflag(int hflag);
int sflag(int sflag);
void oflag(char *string, int oflag, int flag, int *pr, char *pattern,
           int regflags);
void freelist(Regex **head);

int main(int argc, char *argv[]) {
  char lch = '\n';
  int e = 0;
  Regex *head = malloc(sizeof(Regex));
  Regex *now = head;
  FILE *file;
  int flags[10] = {0};
  int i = pars(flags, argc, argv, &now);
  if (flags[0] == 0 && flags[8] == 0) {
    e = 1;
    add(argv[i], &now);
  }
  for (int j = i + e; j < argc; j++) {
    if ((file = fopen(argv[j], "r")) != NULL)
      solut(file, flags, head, argv[j], i + e, argc, &lch);
    else if (sflag(flags[7]) == 1)
      continue;
    else
      printf("%s: No such file or directory\n", argv[j]);
  }
  if (lch != '\n' && flags[3] == 0) printf("\n");
  freelist(&head);
}

void solut(FILE *file, int *flags, Regex *head, char *namefile, int i, int argc,
           char *lch) {
  int ff = 0;
  int flag;
  int *pflag = &flag;
  Regex *start = head;
  int c = 0;
  int n = 0;
  char string[1000];
  int pr = 1;
  int invers = vflag(flags[2]);
  while (fgets(string, 1000, file)) {
    nflag(&n);
    while (head->status != 0) {
      int regflags = REG_EXTENDED;
      if (flags[1] == 1) regflags = REG_EXTENDED | REG_ICASE;
      regex_t regex;
      regcomp(&regex, head->regular, regflags);
      regmatch_t pmatch[1];
      int result = regexec(&regex, string, 1, pmatch, 0);
      *pflag = same(invers, result, &ff);
      oflag(string, flags[9], flag, &pr, head->regular, regflags);
      cflag(flag, &pr, flags[3], &c);
      regfree(&regex);
      if (flag == 1)
        break;
      else
        head = head->next;
    }
    if (flags[4] == 1) {
      head = start;
      continue;
    }
    if (i + 1 != argc && flag == 1 && (hflag(flags[6]) == 0 && flags[3] == 0))
      printf("%s:", namefile);  // выводит названия файла если совпадение и h
                                // флаг выключен и файлов >1
    if (flags[5] == 1 && flag == 1 && (flags[3] != 1 || flags[4] == 1)) {
      if (*lch != '\n') printf("\n");
      printf("%d:", n);
    }
    if (pr == 1 && flag == 1) {
      printf("%s", string);
      *lch = string[strlen(string) - 1];
    }
    head = start;
  }

  if (flags[3] == 1) {
    if (i + 1 != argc) printf("%s:", namefile);
    printf("%d\n", c);
  }
  lflag(ff, flags[4], namefile);
  fclose(file);
}

void oflag(char *string, int oflag, int flag, int *pr, char *pattern,
           int regflags) {
  if (oflag == 1 && flag == 1) {
    *pr = 0;
    regex_t oregex;
    regmatch_t opmatch[1];
    regcomp(&oregex, pattern, regflags);
    while (regexec(&oregex, string, 1, opmatch, 0) == 0) {
      for (int i = opmatch[0].rm_so; i < opmatch[0].rm_eo; i++)
        printf("%c", string[i]);
      printf("\n");
      string += opmatch[0].rm_eo;
    }
    regfree(&oregex);
  }
}

int vflag(int vflag) {
  int invers = 0;
  if (vflag == 1) {
    invers = 1;
  }
  return invers;
}

void cflag(int flag, int *ppr, int cflag, int *c) {
  if (cflag == 1) {
    if (flag == 1) {
      *ppr = 0;
      *c += 1;
    }
  }
}

int lflag(int ff, int lflag, char *namefile) {
  int res = 0;
  if (lflag == 1) {
    if ((ff) == 1) {
      res = 1;
      printf("%s\n", namefile);
    }
  }
  return res;
}

void nflag(int *pn) { *pn += 1; }

int hflag(int hflag) {
  int res = 0;
  if (hflag == 1) res = 1;
  return res;
}

int sflag(int sflag) {
  int res = 0;
  if (sflag == 1) res = 1;
  return res;
}

int same(int invers, int result, int *ff) {
  int res = 0;
  if ((result == 0 && invers == 0) || (invers && result != 0)) {
    res = 1;
    *ff = 1;
  }
  return res;
}
