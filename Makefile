# tools & flags ----------------------------------------------------------------

CXX 		:= c++

CPPFLAGS	= -iquote $(DIRINC)

SPACE		:=
SPACE		+=
VPATH		= $(subst $(SPACE),:,$(shell find $(DIRSRC) -type d))

CXXFLAGS	:= -Wall -Wextra -Werror -std=c++11

NAME 		:= ircserv

RM			:= rm -rf

# filenames & folders ----------------------------------------------------------

DIRSRC		:= src

DIRINC		:= inc

DIROBJ		:= obj

DIRDEP		:= dep

SRC != cat update_src.sh

OBJ = $(addprefix $(DIROBJ)/, $(SRC:.cpp=.o))

MKFILE = $(lastword $(MAKEFILE_LIST))

# primary targets --------------------------------------------------------------

all : $(NAME)

$(NAME) :: $(MKFILE)
	@if [ -f "$@" ]; then $(MAKE) --no-print-directory fclean; fi

$(NAME) :: $(OBJ)
	@$(CXX) $^ -o $@

$(OBJ) :| $(DIROBJ)
$(DEP) :| $(DIRDEP)
$(DIROBJ) $(DIRDEP) &:
	@mkdir -p $(DIROBJ) $(DIRDEP)

include $(shell find $(DIRDEP) -type f 2> /dev/null)

clean :
	@$(RM) $(DIROBJ) $(DIRDEP)

fclean : clean
	@$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re

# build prerequisites ----------------------------------------------------------

$(DIROBJ)/%.o : %.cpp
	@$(CPP) $(CPPFLAGS) $< -MM -MQ $@ -MF $(DIRDEP)/$(*F).d
	@$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
	@echo "|\c"
