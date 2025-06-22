#include <stdlib.h>
#include <unistd.h>


#include <stdio.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 25
#endif



int ft_strlen(char *str)
{
	if(!str)
		return(0);
	int i = 0;
	while(str[i])
		i++;
	return (i);
}

char    *ft_strchr(char *str, char c)
{
	if(!str)
		return (NULL);
	
	while(*str && *str != c)
		str++;
	if(*str == c)		//IMPORTANTE: retornar NULL, si no se encontro c
		return(str);
	else
		return(NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char *new = NULL;
	int	len = 0;
	int i = 0;

	if(!s1 && !s2)
		return(NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	new = malloc(sizeof(char) * (len + 1));
	
	i = 0;
	while(s1 && s1[i])	//comprobar que la cadena existe antes de intentar recorrerla
	{
		new[i] = s1[i];
		i++;
	}
 
	while(s2 && *s2)
	{
		new[i] = *s2;
		i++;
		s2++;
	}
	new[i] = '\0';
	return(new);
}

//retorna una nueva cadena hasta \n o \0 enremind. asigna a remind una
// nueva cadena con el sobrante
char *pickline(char **remind)
{
	char *end = *remind;
	char *remind_aux;
	int i = 0;
	int line_len = 0;
	char *nextline;

	if(*remind == NULL)
		return(NULL);
	//avanzamos puntero end hasta pasado salto de linea, o hata fin del string
	while(*end && *end != '\n')
	{
		end++;
		line_len++;
	}
	if (*end == '\n')
	{
		end++;
		line_len++;
	}

	//creamos la linea a retornar
	nextline = malloc(sizeof(char) * (line_len + 1));
	while(i < line_len)
	{
		nextline[i] = (*remind)[i];
		i++;
	}
	nextline[i] = '\0';
	//en lugra de una cadena vacía retornamos NULL
	if(i == 0) 
	{
		free(nextline);
		nextline = NULL;
	}
	//Actualizamos el resto que almacena la estatica
	remind_aux = malloc(sizeof(char) * (ft_strlen(end) + 1));
	i = 0;
	while(*end)
	{
		remind_aux[i] = *end;
		end++;
		i++;
	}
	remind_aux[i] = '\0';

	free(*remind);
	//si la estatica es una cadena vacia se libera, y se asigna NULL
	if(remind_aux && remind_aux[0] == '\0')
	{
		free(remind_aux);
		remind_aux = NULL;
	
	}
	*remind = remind_aux;

	return(nextline);
}

char *get_next_line(int fd)
{

	if(fd < 0 || BUFFER_SIZE < 1)
		return NULL;
	static char *remind = NULL;
	char *buffer= malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if(!buffer)
		return(NULL);
	buffer[0] = '\0';
	int bytes_read = 1;
	char *tmp;
	

	while(bytes_read > 0 && !ft_strchr(remind, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if(bytes_read < 0)	//liberar estatica si hay fallo en lectura
		{
			if(remind)
				free(remind);
			remind = NULL;
			free(buffer);
			return(NULL);
		}
		buffer[bytes_read] ='\0';
		tmp = ft_strjoin(remind, buffer);
		if(remind)
			free(remind);
		remind = tmp;
	}
	free(buffer);
	return(pickline(&remind));
}

/*
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void test_badfile(void)
{
	int fd;
	char *line;

	printf("\n=== Abriendo archivo inexistente ===\n");
	fd = open("no_existe.txt", O_RDONLY);
	printf(" open retorna: %d \n", fd);
	line = get_next_line(fd);
	if (line == NULL)
		printf("get_next_line retona NULL\n");
	else
		printf("get_next_line FAIL\n");


}

void test_file(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return;
    }

    printf("\n=== Leyendo archivo: %s ===\n", filename);

    char *line;
    int line_num = 1;
    while ((line = get_next_line(fd)))
    {
        printf("[%d] %s", line_num++, line);
        free(line);
    }
    printf("=== EOF alcanzado ===\n");
    close(fd);
}

void test_stdin(void)
{
    printf("\n=== Test con stdin (termina con Ctrl+D) ===\n");
    char *line;
    int line_num = 1;
    while ((line = get_next_line(STDIN_FILENO)))
    {
        printf("[%d] %s", line_num++, line);
        if (!line[0] || line[ft_strlen(line) - 1] != '\n')
            printf("\n");
        free(line);
    }
    printf("=== EOF stdin ===\n");
}

void test_invalidfd(void)
{
	char *line;
	printf("\n===Test con fd invalido===\n");
	while((line=get_next_line(14)))
	{
		printf("%s", line);
		free(line);
	}
}

int main(void)
{
	test_badfile();
    // Casos básicos
    test_file("empty.txt");            // archivo vacío
    test_file("single_line_no_nl.txt");// sin salto final
    test_file("single_line_nl.txt");   // una línea con salto final
    test_file("multi_line.txt");       // varias líneas
    test_file("multi_empty_lines.txt");// líneas vacías
//    test_file("long_line.txt");        // línea muy larga (> BUFFER_SIZE)
//    test_stdin();                      // prueba interactiva con stdin
	test_invalidfd();
    return 0;
}
*/

/*
#include <fcntl.h>

int main(void)
{

	// Test 2: Archivo vacío
	int fd2 = open("empty.txt", O_RDONLY);
	if (fd2 < 0)
	{
		perror("open empty.txt"); return 1;
	}
	printf("\nTest 2: Archivo vacío\n");
	char *line = get_next_line(fd2);
	if (!line)
		printf("-> (null)\n");
	else
	{
		printf("-> %s", line);
		free(line);
	}
	close(fd2);


	// Test 1: Leer de un archivo normal
	int fd1 = open("txt", O_RDONLY);
	if (fd1 < 0)
	{
		perror("open test1.txt"); return 1;
	}
	printf("Test 1: Archivo normal\n");
	int i = 0;
	while ((line = get_next_line(fd1)) != NULL && i < 10)
	{
		i++;
		printf("-> %s", line);
		free(line);
	}
	close(fd1);

	// Test 3: Archivo con una sola línea sin salto final
	int fd3 = open("singleline.txt", O_RDONLY);
	if (fd3 < 0)
	{
		perror("open singleline.txt"); return 1;
	}
	printf("\nTest 3: Una sola línea sin salto final\n");
	while ((line = get_next_line(fd3)) != NULL)
	{
		printf("-> %s", line);
		free(line);
	}
	close(fd3);

	// Test 4: Leer de stdin (descomentar para probar)
	
	printf("\nTest 4: Leer de stdin (Ctrl+D para terminar)\n");
	while ((line = get_next_line(0)) != NULL) {
		printf("-> %s", line);
		free(line);
	}
	

	// Test 5: Archivo con líneas muy largas
	int fd5 = open("longlines.txt", O_RDONLY);
	if (fd5 < 0)
	{
		perror("open longlines.txt"); return 1;
	}
	printf("\nTest 5: Líneas muy largas\n");
	while ((line = get_next_line(fd5)) != NULL)
	{
		printf("-> %s", line);
		free(line);
	}
	close(fd5);

	// Test 6: Archivo con saltos de línea consecutivos
	int fd6 = open("multinewlines.txt", O_RDONLY);
	if (fd6 < 0)
	{
		perror("open multinewlines.txt"); return 1;
	}
	printf("\nTest 6: Saltos de línea consecutivos\n");
	while ((line = get_next_line(fd6)) != NULL)
	{
		printf("-> %s", line);
		free(line);
	}
	close(fd6);

	return 0;
}
*/

/*
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd = open("empty.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return (1);
    }

    char *line = get_next_line(fd);
    if (line == NULL)
        printf("OK: get_next_line devuelve NULL con archivo vacío\n");
    else
    {
        printf("ERROR: get_next_line devuelve: \"%s\"\n", line);
        free(line);
    }

    close(fd);
    return (0);
}
*/
//TEST ft_strjoin
/*int main(void)
{

	char *str;
	str = ft_strjoin("Hola ", "tio.\n");
	printf("%s", str);
	free(str);

	str = ft_strjoin(NULL, "tio.\n");
	printf("%s", str);
	free(str);

	str = ft_strjoin("HOLA \n", NULL);
	printf("%s", str);
	free(str);

	return(0);
}
*/
