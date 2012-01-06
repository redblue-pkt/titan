#ifndef IPKG_H
#define IPKG_H

#define IPKG_LIB
#include "../libipkg/libipkg.h"
#include "../libipkg/args.h"

struct ipkg
{
        char* name;
        char* desc;
        char* version;
        struct ipkg* prev;
        struct ipkg* next;
};

struct ipkg *ipkg = NULL;

void debugipkg()
{
	struct ipkg *node = ipkg;
	
	while(node != NULL)
	{
		if(node->name != NULL)
			printf("pkg name: %s\n", node->name);
		node = node->next;
	}
}

struct ipkg* addipkg(char *name, char* desc, char* version, struct ipkg* last)
{
	//debug(1000, "in");
	struct ipkg *newnode = NULL, *prev = NULL, *node = ipkg;

	newnode = (struct ipkg*)malloc(sizeof(struct ipkg));	
	if(newnode == NULL)
	{
		err("no memory");
		return NULL;
	}
	memset(newnode, 0, sizeof(struct ipkg));

	newnode->name = ostrcat(name, NULL, 0, 0);
	newnode->desc = ostrcat(desc, NULL, 0, 0);
	newnode->version = ostrcat(version, NULL, 0, 0);

	if(last == NULL)
	{
		while(node != NULL)
		{
			prev = node;
			node = node->next;
		}
	}
	else
	{
		prev = last;
		node = last->next;
	}

	if(prev == NULL)
		ipkg = newnode;
	else
	{
		prev->next = newnode;
		newnode->prev = prev;
	}
	newnode->next = node;
	if(node != NULL) node->prev = newnode;

	//debug(1000, "out");
	return newnode;
}

void delipkg(struct ipkg* ipkgnode)
{
	//debug(1000, "in");
	struct ipkg *node = ipkg, *prev = ipkg;

	while(node != NULL)
	{
		if(node == ipkgnode)
		{
			if(node == ipkg)
			{
				ipkg = node->next;
				if(ipkg != NULL)
					ipkg->prev = NULL;
			}
			else
			{
				prev->next = node->next;
				if(node->next != NULL)
					node->next->prev = prev;
			}

			free(node->name);
			node->name = NULL;

			free(node->desc);
			node->desc = NULL;

			free(node->version);
			node->version = NULL;

			free(node);
			node = NULL;
			break;
		}

		prev = node;
		node = node->next;
	}
	//debug(1000, "out");
}

void freeipkg()
{
	debug(1000, "in");
	struct ipkg *node = ipkg, *prev = ipkg;

	while(node != NULL)
	{
		prev = node;
		node = node->next;
		if(prev != NULL)
			delipkg(prev);
	}
	debug(1000, "out");
}

int ipkg_list_cb(char *name, char *desc, char *version, pkg_state_status_t status, void *userdata)
{
	if(desc)
		addipkg(name, desc, version, NULL);
	else
		addipkg(name, NULL, version, NULL);
	return 0;
}

int ipkg_status_cb(char *name, int istatus, char *desc, void *userdata)
{
	addipkg(name, desc, NULL, NULL);
	return 0;
}

int ipkg_update(void)
{
	int err = 0;
	args_t args; 

	args_init(&args);
	err = ipkg_lists_update(&args);
	args_deinit(&args);

	return err;
}

int ipkg_list(void)
{
	int err = 0;
	args_t args;

	args_init(&args);
	err = ipkg_packages_list(&args, NULL, ipkg_list_cb, NULL);
	args_deinit(&args);

	return err;
}

int ipkg_status(const char* package)
{
	int err = 0;
	args_t args;

        args_init(&args);
	err = ipkg_packages_status(&args, package, ipkg_status_cb, NULL);
	args_deinit(&args);

	return err;
}

int ipkg_info(const char* package)
{
	int err = 0;
	args_t args;

        args_init(&args);
	err = ipkg_packages_info(&args, package, ipkg_status_cb, NULL);
	args_deinit(&args);

	return err;
}

int ipkg_install(const char* package)
{
	int err = 0;
	args_t args;

        args_init(&args);
	err = ipkg_packages_install(&args, package);
	args_deinit(&args);

	return err;
}

int ipkg_remove(const char* package, int purge)
{
	int err = 0;
	args_t args;

        args_init(&args);
	err = ipkg_packages_remove(&args, package, purge);
	args_deinit(&args);

	return err;
}

int ipkg_upgrade(void)
{
	int err = 0;
	args_t args;

        args_init(&args);
	err = ipkg_packages_upgrade(&args);
	args_deinit(&args);

	return err;
}

int ipkg_download(const char* package)
{
	int err = 0;
	args_t args;

        args_init(&args);
	err = ipkg_packages_download(&args, package);
	args_deinit(&args);

	return err;
}

int ipkg_files(const char* package)
{
	int err = 0;
	args_t args;

        args_init(&args);
	err = ipkg_package_files(&args, package, ipkg_list_cb, NULL);
	args_deinit(&args);

	return err;
}

int ipkg_search(const char* package)
{
	int err = 0;
	args_t args;

        args_init(&args);
	err = ipkg_file_search(&args, package, ipkg_list_cb, NULL);
	args_deinit(&args);

	return err;
}

#endif

