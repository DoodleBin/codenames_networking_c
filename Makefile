CC := clang
CFLAGS := -g -Wall -Werror

all: codenames_networking

clean:
	@echo "Removing build output"
	@rm -rf codenames_networking.dSYM codenames_networking codenames_networking.zip

myls: myls.c
	$(CC) $(CFLAGS) -o codenames_networking codenames_networking.c

zip:
	@echo "Generating codenames_networking.zip file"
	@zip -q -r codenames_networking.zip . -x .git/\* .vscode/\* .clang-format .gitignore codenames_networking
	@echo "Done. Please upload codenames_networking.zip to Gradescope."

format:
	@echo "Reformatting source code."
	@clang-format -i --style=file $(wildcard *.c) $(wildcard *.h)
	@echo "Done."

.PHONY: all clean zip format