#ifndef IPKG_H
#define IPKG_H

#define IPKG_LIB
#include "libipkg/libipkg.h"
#include "libipkg/args.h"

int ipkg_list_cb(char *name, char *desc, char *version, pkg_state_status_t status, void *userdata)
{
	if(desc)
		printf("%s - %s - %s\n", name, version, desc);
	else
		printf("%s - %s\n", name, version);
	return 0;
}

int ipkg_status_cb(char *name, int istatus, char *desc, void *userdata)
{
	printf("%s\n", desc);
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
