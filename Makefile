CC := clang
CFLAGS := -g -Wall -Werror -Wno-unused-function -Wno-unused-variable
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

all: codenames

clean:
	@echo "Removing build output"
	@rm -rf codenames.dSYM codenames codenames.zip

myls: myls.c
	$(CC) $(CFLAGS) -o codenames codenames.c

zip:
	@echo "Generating codenames.zip file"
	@zip -q -r codenames.zip . -x .git/\* .vscode/\* .clang-format .gitignore codenames
	@echo "Done. Please upload codenames.zip to Gradescope."

format:
	@echo "Reformatting source code."
	@clang-format -i --style=file $(wildcard *.c) $(wildcard *.h)
	@echo "Done."

.PHONY: all clean zip format