import itertools
import random


class Minesweeper():
    """
    Minesweeper game representation
    """

    def __init__(self, height=8, width=8, mines=8):

        # Set initial width, height, and number of mines
        self.height = height
        self.width = width
        self.mines = set()

        # Initialize an empty field with no mines
        self.board = []
        for i in range(self.height):
            row = []
            for j in range(self.width):
                row.append(False)
            self.board.append(row)

        # Add mines randomly
        while len(self.mines) != mines:
            i = random.randrange(height)
            j = random.randrange(width)
            if not self.board[i][j]:
                self.mines.add((i, j))
                self.board[i][j] = True

        # At first, player has found no mines
        self.mines_found = set()

    def print(self):
        """
        Prints a text-based representation
        of where mines are located.
        """
        for i in range(self.height):
            print("--" * self.width + "-")
            for j in range(self.width):
                if self.board[i][j]:
                    print("|X", end="")
                else:
                    print("| ", end="")
            print("|")
        print("--" * self.width + "-")

    def is_mine(self, cell):
        i, j = cell
        return self.board[i][j]

    def nearby_mines(self, cell):
        """
        Returns the number of mines that are
        within one row and column of a given cell,
        not including the cell itself.
        """

        # Keep count of nearby mines
        count = 0

        # Loop over all cells within one row and column
        for i in range(cell[0] - 1, cell[0] + 2):
            for j in range(cell[1] - 1, cell[1] + 2):

                # Ignore the cell itself
                if (i, j) == cell:
                    continue
                if i != cell[0] and j != cell[1]:
                    continue
                # Update count if cell in bounds and is mine
                if 0 <= i < self.height and 0 <= j < self.width:
                    if self.board[i][j]:
                        count += 1

        return count

    def won(self):
        """
        Checks if all mines have been flagged.
        """
        return self.mines_found == self.mines


class Sentence():
    """
    Logical statement about a Minesweeper game
    A sentence consists of a set of board cells,
    and a count of the number of those cells which are mines.
    """

    def __init__(self, cells, count):
        self.cells = set(cells)
        self.count = count

    def __eq__(self, other):
        return self.cells == other.cells and self.count == other.count

    def __str__(self):
        return f"{self.cells} = {self.count}"

    def known_mines(self):
        """
        Returns the set of all cells in self.cells known to be mines.
        """
        if (len(self.cells) == self.count):
            return self.cells
        return set()

    def known_safes(self):
        """
        Returns the set of all cells in self.cells known to be safe.
        """
        if (self.count == 0):
            return self.cells
        return set()
        
    def mark_mine(self, cell):
        """
        Updates internal knowledge representation given the fact that
        a cell is known to be a mine.
        """
        if cell in self.cells:
            self.cells.remove(cell)
            self.count -= 1

    def mark_safe(self, cell):
        """
        Updates internal knowledge representation given the fact that
        a cell is known to be safe.
        """
        
        if cell in self.cells:
            self.cells.remove(cell)


class MinesweeperAI():
    """
    Minesweeper game player
    """

    def __init__(self, height=8, width=8):

        # Set initial height and width
        self.height = height
        self.width = width

        # Keep track of which cells have been clicked on
        self.moves_made = set()

        # Keep track of cells known to be safe or mines
        self.mines = set()
        self.safes = set()

        # List of sentences about the game known to be true
        self.knowledge = []

    def mark_mine(self, cell):
        """
        Marks a cell as a mine, and updates all knowledge
        to mark that cell as a mine as well.
        """
        self.mines.add(cell)
        for sentence in self.knowledge:
            sentence.mark_mine(cell)

    def mark_safe(self, cell):
        """
        Marks a cell as safe, and updates all knowledge
        to mark that cell as safe as well.
        """
        self.safes.add(cell)
        for sentence in self.knowledge:
            sentence.mark_safe(cell)

    def add_knowledge(self, cell, count):
        """
        Called when the Minesweeper board tells us, for a given
        safe cell, how many neighboring cells have mines in them.

        This function should:
            1) mark the cell as a move that has been made
            2) mark the cell as safe
            3) add a new sentence to the AI's knowledge base
               based on the value of `cell` and `count`
            4) mark any additional cells as safe or as mines
               if it can be concluded based on the AI's knowledge base
            5) add any new sentences to the AI's knowledge base
               if they can be inferred from existing knowledge
        """

        self.moves_made.add(cell)
        self.mark_safe(cell)
        ri = [-1, 1, 0, 0]
        ci = [0, 0, 1, -1]
        cells = set()
        for i in range(0, 4):
            x = cell[0] + ri[i]
            y = cell[1] + ci[i]
            if x >= 0 and x < self.height and y >= 0 and y < self.width:
                nc = (x, y)
                if nc in self.safes:
                    continue
                if nc in self.mines:
                    count -= 1
                    continue
                cells.add(nc)

        if len(cells) == 0:
            return
        
        new_sentence = Sentence(cells, count)
        
        new_sentences = [new_sentence]

        i = 0
        while i < len(new_sentences):
            if(len(new_sentences[i].cells) == 0):
                continue
            add = 1
            for j in self.knowledge:
                if(len(j.cells) == 0):
                    continue
                if j == new_sentences[i]:
                    add = 0
                    break
            if add == 1 :
                for j in self.knowledge:
                    if(len(j.cells) == 0):
                        continue
                    if j.cells.issuperset(new_sentences[i].cells) :
                        new_sentence = Sentence(j.cells.difference(new_sentences[i].cells), j.count - new_sentences[i].count)
                        new_sentences.append(new_sentence)
                    elif j.cells.issubset(new_sentences[i].cells) :
                        new_sentence = Sentence(new_sentences[i].cells.difference(j.cells), new_sentences[i].count - j.count)
                        new_sentences.append(new_sentence)
                self.knowledge.append(new_sentences[i])

            i += 1

        for j in self.knowledge:
            for l in j.known_mines().copy():
                if(l not in self.mines):
                    self.mark_mine(l)
            
            for l in j.known_safes().copy():
                if(l not in self.safes):
                    self.mark_safe(l)
        # print('safes, ', self.safes)
        # print('mines, ', self.mines)
            

                

    def make_safe_move(self):
        """
        Returns a safe cell to choose on the Minesweeper board.
        The move must be known to be safe, and not already a move
        that has been made.

        This function may use the knowledge in self.mines, self.safes
        and self.moves_made, but should not modify any of those values.
        """
        for x in self.safes:
            if x not in self.moves_made:
                # print(x)
                return x
        
        return None

    def make_random_move(self):
        """
        Returns a move to make on the Minesweeper board.
        Should choose randomly among cells that:
            1) have not already been chosen, and
            2) are not known to be mines
        """
        remaining = set()
        for i in range(0, self.height):
            for j in range(0, self.width):
                remaining.add((i, j))
        remaining = remaining.difference(self.moves_made)
        remaining = remaining.difference(self.mines)
        if len(remaining) == 0 :
            return None
        return list(remaining)[random.randrange(0, len(remaining), 1)]