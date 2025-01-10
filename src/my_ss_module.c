#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h> 
#include <linux/seq_file.h>
#include <net/tcp.h>
#include <net/udp.h>

const char *tcp_state_to_string(int state) {
    switch (state) {
        case TCP_ESTABLISHED: return "ESTABLISHED";
        case TCP_SYN_SENT: return "SYN_SENT";
        case TCP_SYN_RECV: return "SYN_RECV";
        case TCP_FIN_WAIT1: return "FIN_WAIT1";
        case TCP_FIN_WAIT2: return "FIN_WAIT2";
        case TCP_TIME_WAIT: return "TIME_WAIT";
        case TCP_CLOSE: return "CLOSE";
        case TCP_CLOSE_WAIT: return "CLOSE_WAIT";
        case TCP_LAST_ACK: return "LAST_ACK";
        case TCP_LISTEN: return "LISTEN";
        case TCP_CLOSING: return "CLOSING";
        case TCP_NEW_SYN_RECV: return "NEW_SYN_RECV";
        default: return "UNKNOWN";
    }
}

static int tcp_show(struct seq_file* m, void* v) {
	struct sock *sk;
    struct hlist_nulls_node *node;
    unsigned int bucket;

    seq_printf(m, "%-15s %-8s %-8s %-20s %-20s\n", 
            "State", "Recv-Q", "Send-Q",
            "Local Address:Port", 
            "Remote Address:Port");

    for (bucket = 0; bucket <= tcp_hashinfo.ehash_mask; bucket++) {
        sk_nulls_for_each(sk, node, &tcp_hashinfo.ehash[bucket].chain) {
            struct inet_sock *inet = inet_sk(sk);
            unsigned int recv_q = sk_rmem_alloc_get(sk);
            unsigned int send_q = sk_wmem_alloc_get(sk);

            seq_printf(m, "%-15s %-8u %-8u %-pI4:%-10u %-pI4:%-6u\n",
                    tcp_state_to_string(sk->sk_state), recv_q, send_q,
                    &inet->inet_saddr, ntohs(inet->inet_sport),
                    &inet->inet_daddr, ntohs(inet->inet_dport));
        }
    }

    return 0;
}

static int udp_show(struct seq_file* m, void* v) {
	struct sock *sk;
    struct hlist_nulls_node *node;
    unsigned int bucket;

    seq_printf(m, "%-8s %-8s %-20s %-20s\n", 
            "Recv-Q", "Send-Q",
            "Local Address:Port", 
            "Remote Address:Port");

    for (bucket = 0; bucket <= udp_table.mask; bucket++) {
        sk_for_each(sk, &udp_table.hash[bucket].head) {
            struct inet_sock *inet = inet_sk(sk);
            unsigned int recv_q = sk_rmem_alloc_get(sk);
            unsigned int send_q = sk_wmem_alloc_get(sk);

            seq_printf(m, "%-8u %-8u %-pI4:%-10u %-pI4:%-6u\n",
                    recv_q, send_q, &inet->inet_saddr, 
                    ntohs(inet->inet_sport), &inet->inet_daddr, 
                    ntohs(inet->inet_dport));
        }
    }

    return 0;
}

static int proc_open(struct inode* inode, struct file* file) {
    return single_open(file, udp_show, NULL);
}

static const char* procfs_name = "my_ss_module";
static struct proc_dir_entry *our_proc_file;

static const struct proc_ops proc_file_fops = { 
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
}; 

static int __init procfs_init(void) { 
    our_proc_file = proc_create(procfs_name, 0644, NULL, &proc_file_fops); 
    if (NULL == our_proc_file) { 
        proc_remove(our_proc_file); 
        pr_alert("Error:Could not initialize /proc/%s\n", procfs_name); 
        return -ENOMEM; 
    } 
 
    pr_info("/proc/%s created\n", procfs_name); 
    return 0; 
} 

static void __exit procfs_exit(void) { 
    proc_remove(our_proc_file); 
    pr_info("/proc/%s removed\n", procfs_name); 
} 
 
module_init(procfs_init); 
module_exit(procfs_exit); 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivan Tarasov");
MODULE_DESCRIPTION("Prints information about connections in system (ss command)");