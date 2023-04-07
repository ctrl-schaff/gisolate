#define _DEFAULT_SOURCE

#include "gisolate.h"
#include "set.h"

void create_user_profile(const char* username, user* us)
{
    int total_num_group = get_all_groups(username);
    size_t group_alloc = (size_t)(total_num_group*(int)sizeof(gid_t));

    us->username = username;
    us->num_groups = total_num_group;
    us->allocation = group_alloc;
    set_init(&(us->group_set));
    get_group_data(us);
}

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

void get_group_data(user* us)
{
    struct passwd* pw_data = getpwnam(us->username);
    if (pw_data == NULL)
    {
        printf("Unable to find username %s\n", us->username);
        exit(-1);
    }
    else
    {
        if (us->num_groups > 0)
        {
            gid_t* gdata = (gid_t*)malloc(us->allocation);
            getgrouplist(pw_data->pw_name, pw_data->pw_gid, gdata, &us->num_groups);
            for (int i = 0; i < us->num_groups; ++i)
            {
                const char* group_key = (char*) &gdata[i];
                set_add(&us->group_set, group_key);
            }
            free(gdata);
        }
        else
        {
            printf("No group data found for user %s", us->username);
        }
    }
}

void display_user(const user* us)
{
    printf("User: %s\n", us->username); 
    SimpleSet gset = us->group_set;
    display_set(&gset);
}

void display_set(const SimpleSet* gset)
{
    for (uint64_t i = 0; i < gset->number_nodes; ++i)
    {
        if (gset->nodes[i] != NULL)
        {
            gid_t* group_key = (gid_t*)gset->nodes[i]->_key;
            struct group* gr = getgrgid(*group_key);
            if (gr != NULL)
            {
                printf("\t> %d | %s\n", gr->gr_gid, gr->gr_name);
            }
        }
    }
}

void compare_users(user* us0, user* us1)
{
    SimpleSet setdiff0;
    set_init(&setdiff0);
    set_difference(&setdiff0, &us0->group_set, &us1->group_set);
    display_set(&setdiff0);


    SimpleSet setdiff1;
    set_init(&setdiff1);
    set_difference(&setdiff1, &us0->group_set, &us1->group_set);
    display_set(&setdiff1);
}

int main(int argc, char** argv)
{
    char* usernames[2] = {NULL, NULL};
    unsigned int user_count = 0;

    int command = -1;
    while ((command = getopt(argc, argv, "u:")) != -1)
    {
        switch(command)
        {
            case 'u':
                if (user_count < 2)
                {
                    usernames[user_count] = optarg;
                    ++user_count;
                }
                break;
            case '?':
                if(optopt == 'u')
                {
                    fprintf(stderr, "Option -%u requires an argument\n", optopt);
                }
                return 1;
            default:
                abort();
        }
    }

    user us0;
    create_user_profile(usernames[0], &us0);
    display_user(&us0);

    user us1;
    create_user_profile(usernames[1], &us1);
    display_user(&us1);

    compare_users(&us0, &us1);

    set_destroy(&us0.group_set);
    set_destroy(&us1.group_set);
}
