#include <iomanip>
#include <easy_tcp.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <params_cpp.h>

using namespace easy_tcp;
using namespace std::chrono_literals;
using namespace std;
using namespace params_cpp;

int main (int argc, char **argv){
    Parser p(argc,argv);


    auto check_point = std::chrono::high_resolution_clock::now();

    auto file_name = p.get(Key("-f","--file"),"");
    auto ip = p.get(Key("-i","--ip"),"");
    auto port = stoi(p.get(Key("-p","--port"),""));

    uint32_t size = 0;
    {
        std::fstream motd(file_name.c_str(), std::ios::binary | std::ios::in | std::ios::ate);
        if (!motd.is_open()) {
            cout << "File not found." << endl;
            exit(1);
        }
        size = motd.tellg();
    }
    std::ifstream fin(file_name.c_str(), std::ios::in | std::ios::binary );
    std::vector<char> buffer(size);
    fin.read(buffer.data(), size);

    Client c;
    cout << "Connecting to ESP32... ";
    if (!c.connect(ip, port)){
        cout << "Fail (no response)" << endl;
        exit(1);
    }
    Connection &conn =  c;
    uint32_t chunk_size = 0;
    conn.send_data((char *) &size, sizeof(size));
    if (!conn.wait_for_data(chunk_size, 10s)){
        cout << "Fail (wrong response)" << endl;
        exit(1);
    }
    cout << "Success" << endl;
    unsigned int chunk_count = size / chunk_size;
    uint32_t i ;
    uint32_t confirmation;
    for (i = 0;i < chunk_count; i++) {
        std::stringstream stream;
        stream << std::setfill ('0') << std::setw(8) << std::hex <<  i * chunk_size ;
        cout << "Writing at 0x" <<  stream.str() << "... (" << ( i * 100 / chunk_count ) << "%)" << endl;
        conn.send_data(buffer.data() + i * chunk_size, chunk_size);
        if (!conn.wait_for_data(confirmation, 10s)){
            cout << "failed to received confirmation";
            exit(1);
        }
    }
    conn.send_data(buffer.data() + i * chunk_size, size % chunk_size);
    std::stringstream stream;
    stream << std::hex << i * chunk_size;
    cout << "Writing at " <<  stream.str() << "... (100%)" << endl;
    auto time_taken = (std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - check_point).count()) / 1000.0;
    cout << "Wrote " << size << " bytes in " << time_taken << " seconds (effective " << size / 1000 / time_taken << " kbit/s)..." << endl;
    cout << "Forcing reset..." << endl;
    cout << "Done!" << endl;
    c.disconnect();
}