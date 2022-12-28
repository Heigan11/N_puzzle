
NAME		=	n-puzzle
INC_DIR		=	includes
SRC_DIR		=	srcs
CC			=	g++
# CC			=	clang++

SRCS		=	$(SRC_DIR)/main.cpp \
				$(SRC_DIR)/Parser.cpp \
				$(SRC_DIR)/Puzzle.cpp \
				$(SRC_DIR)/PuzzleData.cpp  \
				$(SRC_DIR)/Runner.cpp

OBJS		=	$(SRCS:.cpp=.o)

INC			=	-I$(INC_DIR)

HEAD		=	$(INC_DIR)/Parser.hpp \
				$(INC_DIR)/Puzzle.hpp \
				$(INC_DIR)/PuzzleData.hpp \
				$(INC_DIR)/Runner.hpp \
				$(INC_DIR)/State.hpp \

CFLAGS		=	-Wextra -Werror -Wall $(INC) -std=c++11

OPTFLAGS	=	-O0

MAKE		=	make

RM			=	rm -rf

.PHONY: all clean fclean re

all:
	@$(MAKE) $(NAME) -j 4

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OPTFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

gen3s:
	python3 ./additional/npuzzle_gen.py 3 -s > puzzle3.puzzle

gen3u:
	python3 ./additional/npuzzle_gen.py 3 -u > puzzle3.puzzle

gen4s:
	python3 ./additional/npuzzle_gen.py 4 -s > puzzle4.puzzle

gen4u:
	python3 ./additional/npuzzle_gen.py 4 -u > puzzle4.puzzle

%.o:%.cpp $(HEAD)
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $< -o $@

