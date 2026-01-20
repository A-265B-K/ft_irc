$(shell rm -f .step_state)

# tools & flags ----------------------------------------------------------------

CXX 		:= c++

CPPFLAGS	= -I $(DIRINC)

SPACE		:=
SPACE		+=
VPATH		= $(subst $(SPACE),:,$(shell find $(DIRSRC) -type d))

CXXFLAGS	:= -Wall -Wextra -Werror -std=c++11 -fsanitize=address

NAME 		:= ircserv

RM			:= rm -rf

# filenames & folders ----------------------------------------------------------

DIRSRC		:= src

DIRINC		:= inc

DIROBJ		:= obj

DIRDEP		:= dep

SRC			!= cat src.list

OBJ			= $(addprefix $(DIROBJ)/, $(SRC:.cpp=.o))

MKFILE		= $(lastword $(MAKEFILE_LIST))

# colours



# primary targets --------------------------------------------------------------

all : $(NAME)

$(NAME) :: $(MKFILE)
	@if [ -f "$@" ]; then $(MAKE) --no-print-directory fclean; fi

$(NAME) :: $(OBJ)
	@$(CXX) $^ -o $@ $(CXXFLAGS)
	@echo
	@echo "$(NAME) built!"

$(OBJ) :| $(DIROBJ)
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
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -MF $(DIRDEP)/$(*F).d -c $< -o $@
	@./progress_bar.sh
