#define _DEFAULT_SOURCE

#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

struct user_group {
    const char* username;
    gid_t* group_data;
    int num_groups;
};

int get_all_groups(const char* username)
{
    struct passwd* pw_data = getpwnam(username);
    if (pw_data == NULL)
    {
        printf("Unable to find username %s\n", username);
        return -1;
    }
    else
    {
        int ngroups = 0;
        getgrouplist(pw_data->pw_name, pw_data->pw_gid, NULL, &ngroups);
        return ngroups;
    }
}
void get_group_data(struct user_group* ug)
{
    struct passwd* pw_data = getpwnam(ug->username);
    if (pw_data == NULL)
    {
        printf("Unable to find username %s\n", ug->username);
        exit(-1);
    }
    else
    {
        int num_groups = get_all_groups(ug->username);
        if (num_groups > 0)
        {
            size_t group_alloc = (size_t)(num_groups*(int)sizeof(gid_t));
            ug->group_data = malloc(group_alloc);
            getgrouplist(pw_data->pw_name, pw_data->pw_gid, ug->group_data, &num_groups);
            ug->num_groups = num_groups;
        }
        else
        {
            printf("No group data found for user %s", ug->username);
            ug->group_data = NULL;
        }
    }
}

void display_user_group_data(const struct user_group* ug)
{
    printf("User: %s\n", ug->username);
    for (int i = 0; i < ug->num_groups; ++i) 
    {
        struct group* gr = getgrgid(ug->group_data[i]);
        if (gr != NULL)
        {
            printf("\t> %d | %s\n", gr->gr_gid, gr->gr_name);
        }
    }
}

int main()
{
    struct user_group ug = {
        .username = "schaffj",
        .num_groups = 0
    };
    get_group_data(&ug);
    display_user_group_data(&ug);
    free(ug.group_data);
}
