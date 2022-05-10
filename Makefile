CMD     = @

WARNING = -Wall -Wpedantic -Wextra -Wold-style-definition \
	  -Winit-self -Wfloat-equal -Wstrict-prototypes -Wredundant-decls \
	  -Wendif-labels -Wstrict-aliasing=2 -Woverflow -Wformat=2 -Wtrigraphs \
	  -Wmissing-include-dirs -Wno-format-nonliteral -Wunused-parameter \
	  -Wincompatible-pointer-types \
	  -Werror=implicit-function-declaration -Werror=return-type
CC      = cc
CFLAGS  = -Og -ggdb $(WARNING)

%.o: %.c
	@printf "    %-8s%s\n" "CC" $@
	$(CMD)$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: check
check: test
	$(CMD)./test

.PHONY: clean
clean:
	rm -f test

test: test.c
	@printf "    %-8s%s\n" "CCLD" $@
	$(CMD)$(CC) $< -o $@ $(CFLAGS)
