#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/**********************************ACCESS******************************************
Elle sert à vérifier si un fichier ou un exécutable est accessible (lecture,
	écriture, exécution) selon les droits du processus courant.
***********************************************************************************/

/*
pathname → chemin vers le fichier ou la commande

mode :

	F_OK : tester si le fichier existe

	R_OK : tester si lecture autorisée

	W_OK : tester si écriture autorisée

	X_OK : tester si exécution autorisée (important pour pipex)

📌 Utilisation dans pipex

	Quand tu dois exécuter "ls -l" :

	Tu vas parser la commande "ls"

	Parcourir tous les dossiers du $PATH

	Tester chaque dossier avec access(path_to_ls, X_OK)
*/

int	main(void)
{
	const char	*cmd = "/bin/ls";

	if (access(cmd, X_OK) == 0)
		printf("✅ La commande '%s' est exécutable.\n", cmd);
	else
		perror("❌ Erreur");
	return (0);
}

/*****************************FORK**********************************
	fork() sert à créer un nouveau processus : on fait une copie du processus courant.

	Le processus père continue à s’exécuter.

	Le nouveau processus (le fils) a son propre PID,
		mais il partage le même code et la même mémoire au moment de la création.

	🔸 Valeur de retour :

	== 0 : tu es dans le fils

	> 0 : tu es dans le père (la valeur est le pid du fils)

	< 0 : échec du fork

		1 fork() pour cmd1

	1 fork() pour cmd2

Chaque enfant redirige ses entrées/sorties avec dup2,
	puis exécute une commande avec execve.
*/

int	main(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		printf("👶 Je suis le fils (pid: %d)\n", getpid());
	else
		printf("🧔 Je suis le père (pid: %d), mon fils a pour pid: %d\n",
			getpid(), pid);
	return (0);
}

/************************FORK + EXECVE************************
Créer un nouveau processus (avec fork) pour qu’il exécute une commande (avec execve) sans bloquer le processus principal.


🧩 Étapes du code :

	Le fork crée un clone du programme

	Le fils exécute "ls -l" via execve

	Le père attend avec wait()

	Le programme principal continue quand la commande est terminée

🧨 Ce qu’il faut absolument comprendre
🧠 Concept	💬 Explication
execve remplace	Si execve réussit, le reste du code après n’est jamais exécuté
envp est requis	Il faut passer l’environnement (ou NULL si pas besoin de variables comme PATH)
args[0] = nom de la commande	Toujours mettre "ls" même si le chemin est "/bin/ls"
Le père attend
*/

int	main(int ac, char **av, char **envp)
{
	pid_t	pid;
	char	*cmd_path;
	char	*args[] = {"ls", "-l", NULL};

	cmd_path = "/bin/ls";
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		// 👶 Processus enfant : on exécute ls -l
		if (execve(cmd_path, args, envp) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	else
	{
		// 🧔 Processus parent : on attend que le fils finisse
		wait(NULL);
		printf("✅ Commande terminée.\n");
	}
	return (0);
}

/*********************************DUP2**********************************************

🔹 À quoi sert dup2 ?

dup2(oldfd, newfd) redirige un descripteur de fichier.

	Il dit : “Fais en sorte que newfd pointe vers la même chose que oldfd.”


| Cas           | Ce qu’il fait                            |
| ------------- | ---------------------------------------- |
| `dup2(fd, 0)` | Redirige **l’entrée standard** (stdin)   |
| `dup2(fd, 1)` | Redirige **la sortie standard** (stdout) |
| `dup2(fd, 2)` | Redirige **la sortie d’erreur** (stderr) |



📌 Utilisation dans pipex

Quand tu veux faire :

< infile cmd1 | cmd2 > outfile

	Dans cmd1 :

dup2(infile_fd, 0); // redirige stdin
dup2(pipe_fd[1], 1); // redirige stdout vers l'entrée du pipe

Dans cmd2 :

dup2(pipe_fd[0], 0); // redirige stdin depuis la sortie du pipe
dup2(outfile_fd, 1); // redirige stdout vers outfile
*/

int	main(void)
{
	int	fd;

	fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (1);
	dup2(fd, 1); // stdout → output.txt
	close(fd);   // bonne pratique : on ferme l’original
	printf("Ceci ira dans output.txt !\n");
	return (0);
}

/*************************************PIPE************************************

pipe() permet de connecter deux processus entre eux, pour que :

Ce que l’un écrit soit lu par l’autre.

On appelle ça un tube (pipe) :

cmd1 | cmd2

	cmd1 écrit dans le pipe

	cmd2 lit ce que cmd1 a écrit

	fd[0] → extrémité lecture

fd[1] → extrémité écriture

			┌─────────────┐
			│  pipe fd[1] │  ← cmd1 écrit ici (stdout)
			│      ↓      │
			│    buffer   │
			│      ↓      │
			│  pipe fd[0] │  ← cmd2 lit ici (stdin)
			└─────────────┘



🧠 Utilisation typique dans pipex

Dans cmd1 | cmd2 :

	Le processus de cmd1 fait :

dup2(fd[1], 1); // redirige stdout vers le pipe

Le processus de cmd2 fait :

dup2(fd[0], 0); // redirige stdin depuis le pipe


*/

int	main(void)
{
	int fd[2];
	pipe(fd);

	write(fd[1], "hello\n", 6); // écrire dans le tube
	char buffer[100];
	read(fd[0], buffer, 100); // lire depuis le tube

	write(1, buffer, 6); // afficher ce qu'on a lu

	return (0);
}