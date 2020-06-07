import random

empty_box = " "
crossword_grid = []

def generate_crossword(solution_words):
    global crossword_grid

    grid_size = 0
    max_queue_limit = len(solution_words) * 100
    queued_words_count = 0
    # find the word with max characters
    for word in solution_words:
        if len(word) > grid_size:
            grid_size = len(word)

    crossword_grid = [[empty_box] * grid_size] * grid_size

    # first word will be placed randomly
    pos_x = random.randint(0, grid_size-1)
    pos_y = random.randint(0, grid_size-1)
    # placing the longest words first in hopes of generating maximum intersections
    solution_words = sort_according_to_length(solution_words)
    curr_word = solution_words[0]
    solution_words.remove(curr_word)
    is_place_horizontally = True if random.randint(0, 1) == 0 else False

    first_word_place_check_counter = 0
    while (not place_word_in_grid((pos_x, pos_y), curr_word, is_place_horizontally)):
        first_word_place_check_counter += 0.1
        decrement_value = int(first_word_place_check_counter)
        pos_x = random.randint(0, grid_size-1 - decrement_value)
        pos_y = random.randint(0, grid_size-1 - decrement_value)
        is_place_horizontally = True if random.randint(0, 1) == 0 else False

    while (len(solution_words) > 0):
        # we take the first value in the list, so it behaves like a queue
        curr_word = solution_words[0]
        common_char = ''
        curr_word_chars = list(set(curr_word))
        positions_to_check = []
        is_word_placed_successfully = False

        while (len(positions_to_check) == 0):
            if (len(curr_word_chars) == 0): break

            common_char = random.choice(curr_word_chars)
            curr_word_chars.remove(common_char)
            common_char_offset = curr_word.index(common_char)
            positions_to_check = find_char(common_char)

        while (len(positions_to_check) > 0):
            random_position = random.choice(positions_to_check)
            positions_to_check.remove(random_position)

            is_place_horizontally = True if random.randint(0, 1) == 0 else False
            #print ('Trying to put ', curr_word, ' at (', random_position[0], ', ', random_position[1], ')', ' horizontally.' if is_place_horizontally else 'vertically')
            if resize_grid_and_place_word(random_position, common_char_offset, curr_word, is_place_horizontally):
                solution_words.remove(curr_word)
                break

            is_place_horizontally = not is_place_horizontally
            if resize_grid_and_place_word(random_position, common_char_offset, curr_word, is_place_horizontally):
                solution_words.remove(curr_word)
                break

        #print('counldnt place word')
        if len(positions_to_check) == 0 and curr_word in solution_words:
            if queued_words_count < max_queue_limit:
                # if a position could not be found for the word, enqueue it again
                queued_word = ''
                queued_word = solution_words.pop(solution_words.index(curr_word))
                solution_words.append(queued_word)
                queued_words_count += 1
            else:
                # forcing the word somewhere
                unique_word_place_check_counter = 0
                while (not is_word_placed_successfully):
                    #print ('Trying to put unique word ', curr_word, ' at (', pos_x, ', ', pos_y, ')', 'horizontally' if is_place_horizontally else ' vertically')
                    unique_word_place_check_counter += 0.1
                    increment_value = int(unique_word_place_check_counter)
                    pos_x = random.randint(0, grid_size-1 + increment_value)
                    pos_y = random.randint(0, grid_size-1 + increment_value)
                    is_place_horizontally = True if random.randint(0,1) == 0 else False
                    is_word_placed_successfully = resize_grid_and_place_word((pos_x, pos_y), 0, curr_word, is_place_horizontally)
                solution_words.remove(curr_word)

    #print("\n\n============== final grid =============")
    trim_edges()
    print_grid(crossword_grid)

def resize_grid_and_place_word(random_position, common_char_offset, word_to_place, is_place_horizontally = True):
    global crossword_grid
    pos_x = 0
    pos_y = 0

    if (is_place_horizontally):
        pos_x = random_position[0]
        pos_y = random_position[1] - common_char_offset

        if (pos_y < 0):
            resize_offset = abs(pos_y)
            crossword_grid = resize_grid(resize_offset, "TL")
            pos_y += resize_offset

        if (pos_y + len(word_to_place) > len(crossword_grid)):
            resize_offset = abs(pos_y + len(word_to_place) - len(crossword_grid))
            crossword_grid = resize_grid(resize_offset, "BR")
    else:
        pos_x = random_position[0] - common_char_offset
        pos_y = random_position[1]

        if (pos_x < 0):
            resize_offset = abs(pos_x)
            crossword_grid = resize_grid(resize_offset, "TL")
            pos_x += resize_offset
        if (pos_x + len(word_to_place) > len(crossword_grid)):
            resize_offset = abs(pos_x + len(word_to_place) - len(crossword_grid))
            crossword_grid = resize_grid(resize_offset, "BR")

    if (place_word_in_grid((pos_x, pos_y), word_to_place, is_place_horizontally)):
        return True
    else:
        return False

def resize_grid(size_difference, side):
    global crossword_grid
    old_grid_size = len(crossword_grid)
    new_grid_size = len(crossword_grid) + size_difference
    new_grid = [[empty_box] * new_grid_size] * new_grid_size
    row_size_difference = column_size_difference = 0

    if side == "TL":
        row_size_difference = size_difference
        column_size_difference = size_difference
    elif side == "TR":
        row_size_difference = size_difference
        column_size_difference = 0
    elif side == "BL":
        row_size_difference = 0
        column_size_difference = size_difference
    elif side == "BR":
        row_size_difference = 0
        column_size_difference = 0
    else:
        print ('Invalid side')
        return

    for i in range(old_grid_size):
        for j in range(old_grid_size):
            new_row = new_grid[i + row_size_difference].copy()
            new_row[j + column_size_difference] = crossword_grid[i][j]
            new_grid[i + row_size_difference] = new_row

    return new_grid

def place_word_in_grid(position_tuple, word, is_horizontal=True):
    global crossword_grid
    word_iterator = 0
    grid_size = len(crossword_grid)
    if not check_if_pos_in_grid(len(crossword_grid), position_tuple):
        return False

    #print("Trying to place ", word, " at (", position_tuple[0], ", ", position_tuple[1], "). Horizontally? ", is_horizontal)
    if (is_horizontal):
        if (position_tuple[1] + len(word) <= grid_size):
            if (check_if_empty_or_needed_letter(position_tuple, word, True)):
                new_row = crossword_grid[position_tuple[0]].copy()
                for i in range(position_tuple[1], position_tuple[1] + len(word)):
                    new_row[i] = word[word_iterator]
                    word_iterator += 1

                crossword_grid[position_tuple[0]] = new_row
            else:
                #print("No free or needed boxes 1")
                return False
        else:
            #print("Can't fit the word in given position")
            return False
    else:
        if (position_tuple[0] + len(word) <= grid_size):
            if (check_if_empty_or_needed_letter(position_tuple, word, False)):
                for i in range(position_tuple[0], position_tuple[0]+len(word)):
                        new_row = crossword_grid[i].copy()
                        new_row[position_tuple[1]] = word[word_iterator]

                        crossword_grid[i] = new_row
                        word_iterator += 1
            else:
                #print("No free or needed boxes 2")
                return False
        else:
            #print("Can't fit the word in given position")
            return False
    return True

def check_if_empty_or_needed_letter(position_tuple, word, is_horizontal=True):
    global crossword_grid
    pos_x = position_tuple[0]
    pos_y = position_tuple[1]
    is_empty_or_needed = False

    if (is_horizontal):
        for c in range(len(word)):
            if (crossword_grid[pos_x][pos_y] == empty_box \
                    and crossword_grid[pos_x-1 if pos_x > 0 else pos_x][pos_y] == empty_box \
                    and crossword_grid[pos_x+1 if pos_x < len(crossword_grid)-1 else pos_x][pos_y] == empty_box) \
                    or crossword_grid[pos_x][pos_y] == word[c]:
                is_empty_or_needed = True
            else:
                is_empty_or_needed = False
                return is_empty_or_needed

            if (c == 0 and pos_y > 0):
                if (crossword_grid[pos_x][pos_y-1] != empty_box):
                    is_empty_or_needed = False
                    return is_empty_or_needed
            if (c == len(word)-1 and pos_y < len(crossword_grid)-1):
                if (crossword_grid[pos_x][pos_y+1] != empty_box):
                    is_empty_or_needed = False
                    return is_empty_or_needed

            pos_y += 1
            if pos_y > len(crossword_grid):
                break
    else:
        for c in range(len(word)):
            if (crossword_grid[pos_x][pos_y] == empty_box \
                    and crossword_grid[pos_x][pos_y-1 if pos_y > 0 else pos_y] == empty_box \
                    and crossword_grid[pos_x][pos_y+1 if pos_y < len(crossword_grid)-1 else pos_y] == empty_box) \
                    or crossword_grid[pos_x][pos_y] == word[c]:
                is_empty_or_needed = True
            else:
                is_empty_or_needed = False
                return is_empty_or_needed

            if (c == 0 and pos_x > 0):
                if (crossword_grid[pos_x-1][pos_y] != empty_box):
                    is_empty_or_needed = False
                    return is_empty_or_needed
            if (c == len(word)-1 and pos_x < len(crossword_grid)-1):
                if (crossword_grid[pos_x+1][pos_y] != empty_box):
                    is_empty_or_needed = False
                    return is_empty_or_needed

            pos_x += 1
            if (pos_x > len(crossword_grid)):
                break

    return is_empty_or_needed

def print_grid(grid):
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            print(grid[i][j], end=" ")
        print()
    return

def check_if_pos_in_grid(grid_size, position_tuple):
    if position_tuple[0] >= 0 and position_tuple[0] <= grid_size and position_tuple[1] >= 0 and position_tuple[1] <= grid_size:
        return True
    else:
        return False

def find_char(char_to_find):
    global crossword_grid
    grid_size = len(crossword_grid)
    found_char_positions = []
    for x in range(grid_size):
        for y in range(grid_size):
            if crossword_grid[x][y] == char_to_find:
                found_char_positions.append((x, y))

    return found_char_positions

def scramble_list(list_to_scramble):
    new_list = []
    for i in range(len(list_to_scramble)):
        temp_item = random.choice(list_to_scramble)
        new_list.append(temp_item)
        list_to_scramble.remove(temp_item)

    return new_list

def sort_according_to_length(list_to_sort):
    new_list = []
    while len(list_to_sort) > 0:
        longest_word = ''
        for word in list_to_sort:
            if len(word) > len(longest_word):
                longest_word = word
        new_list.append(longest_word)
        list_to_sort.remove(longest_word)
    return new_list

def trim_edges():
    global crossword_grid
    trim_size_top = 0
    trim_size_bottom = 0
    trim_size_left = 0
    trim_size_right = 0

    # trimming top rows
    is_empty_line = False
    for row in crossword_grid:
        for j in row:
            if j == empty_box:
                is_empty_line = True
            else:
                is_empty_line = False
                break

        if is_empty_line:
            trim_size_top += 1
        else:
            break
    crossword_grid = trim_side(trim_size_top, "T")

    # trimming bottom rows
    is_empty_line = False
    for i in range(len(crossword_grid)-1, -1, -1):
        for j in range(len(crossword_grid[i])):
            if (crossword_grid[i][j] == empty_box):
                is_empty_line = True
            else:
                is_empty_line = False
                break

        if is_empty_line:
            trim_size_bottom += 1
        else:
            break

    crossword_grid = trim_side(trim_size_bottom, "B")

    # trimming left columns
    is_empty_line = False
    for i in range(len(crossword_grid[0])):
        for j in range(len(crossword_grid)):
            if (crossword_grid[j][i] == empty_box):
                is_empty_line = True
            else:
                is_empty_line = False
                break

        if is_empty_line:
            trim_size_left += 1
        else:
            break

    crossword_grid = trim_side(trim_size_left, "L")

    # trimming right columns
    is_empty_line = False
    for i in range(len(crossword_grid[0])-1, -1, -1):
        for j in range(len(crossword_grid)):
            if (crossword_grid[j][i] == empty_box):
                is_empty_line = True
            else:
                is_empty_line = False
                break

        if is_empty_line:
            trim_size_right += 1
        else:
            break

    crossword_grid = trim_side(trim_size_right, "R")

def trim_side(trim_size, side_to_trim):
    global crossword_grid
    new_grid_row_size = 0
    new_grid_column_size = 0

    if side_to_trim == "T":
        new_grid_row_size = len(crossword_grid)-trim_size
        new_grid_column_size = len(crossword_grid[0])
        new_grid = [[empty_box] * new_grid_column_size] * new_grid_row_size

        for x in range(new_grid_row_size):
            new_grid[x] = crossword_grid[x+trim_size]
    elif side_to_trim == "B":
        new_grid_row_size = len(crossword_grid)-trim_size
        new_grid_column_size = len(crossword_grid[0])
        new_grid = [[empty_box] * new_grid_column_size] * new_grid_row_size

        for x in range(new_grid_row_size):
            new_grid[x] = crossword_grid[x]
    elif side_to_trim == "L":
        new_grid_row_size = len(crossword_grid)
        new_grid_column_size = len(crossword_grid[0]) - trim_size
        new_grid = [[empty_box] * new_grid_column_size] * new_grid_row_size

        for x in range(new_grid_row_size):
            new_row = crossword_grid[x][trim_size:]
            new_grid[x] = new_row
    elif side_to_trim == "R":
        new_grid_row_size = len(crossword_grid)
        new_grid_column_size = len(crossword_grid[0]) - trim_size

        new_grid = [[empty_box] * new_grid_column_size] * new_grid_row_size

        for x in range(new_grid_row_size):
            new_row = crossword_grid[x][:new_grid_column_size]
            new_grid[x] = new_row

    return new_grid

input_words = ["female", "spotless", "arrange", "equal", "penultimate", "equanimity", "circumlocution", "auspicious", "acumen", "magnanimous"]
generate_crossword(input_words)

