#include "shell.h"

/**
 * input_buf - Reads and buffers chained commands.
 * @info: Parameter struct
 * @buffer: Address of the buffer
 * @buffer_length: Address of buffer length variable
 *
 * Return: Number of bytes read
 */
ssize_t input_buf(info_tree *info, char **buffer, size_t *buffer_length)
{
    ssize_t bytes_read = 0;
    size_t line_length = 0;

    if (!*buffer_length) /* If nothing left in the buffer, fill it */
    {
        /*sj_buffree((void **)info->cmd_buf);*/
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, sj_intvalue);

#if USEsj_get_line_info
        bytes_read = getline(buffer, &line_length, stdin);
#else
        bytes_read = sj_get_line_info(info, buffer, &line_length);
#endif

        if (bytes_read > 0)
        {
            if ((*buffer)[bytes_read - 1] == '\n')
            {
                (*buffer)[bytes_read - 1] = '\0'; /* Remove trailing newline */
                bytes_read--;
            }

            info->linecount_flag = 1;
            sj_rm_comment(*buffer);
            sj_cost_hfile(info, *buffer, info->histcount++);
            /* if (sj_stricatchr(*buffer, ';')) Is this a command chain? */
            {
                *buffer_length = bytes_read;
                info->cmd_buf = buffer;
            }
        }
    }

    return bytes_read;
}

/**
 * sj_get_value - Gets a line minus the newline.
 * @info: Parameter struct
 *
 * Return: Number of bytes read
 */
ssize_t sj_get_value(info_tree *info)
{
    static char *command_buffer = NULL; /* The ';' command chain buffer */
    static size_t current_pos, iterator, buffer_length;
    ssize_t bytes_read = 0;
    char **command_buffer_ptr = &(info->arg);
    char *current_command;

    sj_putchar(BUF_FLUSH);
    bytes_read = input_buf(info, &command_buffer, &buffer_length);

    if (bytes_read == -1) /* EOF */
        return -1;

    if (buffer_length) /* We have commands left in the chain buffer */
    {
        iterator = current_pos; /* Initialize new iterator to the current buffer position */
        current_command = command_buffer + current_pos; /* Get a pointer for return */

        sj_get_chainvalue(info, command_buffer, &iterator, current_pos, buffer_length);

        while (iterator < buffer_length) /* Iterate to semicolon or the end */
        {
            if (sj_chain_status(info, command_buffer, &iterator))
                break;
            iterator++;
        }

        current_pos = iterator + 1; /* Increment past the nulled ';' */
        if (current_pos >= buffer_length) /* Reached the end of the buffer? */
        {
            current_pos = buffer_length = 0; /* Reset position and length */
            info->cmd_buf_type = CMD_NORM;
        }

        *command_buffer_ptr = current_command; /* Pass back a pointer to the current command position */
        return sj_get_strlen(current_command); /* Return the length of the current command */
    }

    *command_buffer_ptr = command_buffer; /* Otherwise, it's not a chain, pass back the buffer from sj_get_line_info() */
    return bytes_read; /* Return the length of the buffer from sj_get_line_info() */
}

/**
 * read_buf - Reads a buffer.
 * @info: Parameter struct
 * @buffer: Buffer
 * @size: Size
 *
 * Return: Number of bytes read
 */
ssize_t read_buf(info_tree *info, char *buffer, size_t *size)
{
    ssize_t bytes_read = 0;

    if (*size)
        return 0;

    bytes_read = read(info->readindex, buffer, READ_BUF_SIZE);

    if (bytes_read >= 0)
        *size = bytes_read;

    return bytes_read;
}

/**
 * sj_get_line_info - Gets the next line of input from STDIN.
 * @info: Parameter struct
 * @buffer_ptr: Address of a pointer to the buffer, preallocated or NULL
 * @buffer_length: Size of preallocated buffer if not NULL
 *
 * Return: Number of bytes read
 */
int sj_get_line_info(info_tree *info, char **buffer_ptr, size_t *buffer_length)
{
    static char read_buffer[READ_BUF_SIZE];
    static size_t current_pos, length;
    size_t new_size;
    ssize_t bytes_read = 0;
    int total_bytes_read = 0;
    char *buffer = NULL, *new_buffer = NULL, *newline_position;

    buffer = *buffer_ptr;
    if (buffer && buffer_length)
        total_bytes_read = *buffer_length;
    if (current_pos == length)
        current_pos = length = 0;

    bytes_read = read_buf(info, read_buffer, &length);

    if (bytes_read == -1 || (bytes_read == 0 && length == 0))
        return -1;

    newline_position = sj_stricatchr(read_buffer + current_pos, '\n');
    new_size = newline_position ? 1 + (unsigned int)(newline_position - read_buffer) : length;

    new_buffer = sj_re_allocate(buffer, total_bytes_read, total_bytes_read ? total_bytes_read + new_size : new_size + 1);
    
    if (!new_buffer) /* MALLOC FAILURE! */
        return (buffer ? free(buffer), -1 : -1);

    if (total_bytes_read)
        sj_striconcat(new_buffer, read_buffer + current_pos, new_size - current_pos);
    else
        sj_stricpindex(new_buffer, read_buffer + current_pos, new_size - current_pos + 1);

    total_bytes_read += new_size - current_pos;
    current_pos = new_size;
    buffer = new_buffer;

    if (buffer_length)
        *buffer_length = total_bytes_read;
    *buffer_ptr = buffer;

    return total_bytes_read;
}

/**
 * sj_intvalue - Blocks Ctrl-C.
 * @sig_num: The signal number
 *
 * Return: void
 */
void sj_intvalue(__attribute__((unused))int sig_num)
{
    sj_putstr("\n");
    sj_putstr("$ ");
    sj_putchar(BUF_FLUSH);
}
