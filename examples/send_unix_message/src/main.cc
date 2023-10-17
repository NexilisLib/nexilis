#include <nexilis/af_unix/unix_socket_sender.hh>

int main()
{
    nexilis::UnixSocketSender sender("/tmp/nexilis");
    sender.sendMessage("moika");

    return 0;
}
