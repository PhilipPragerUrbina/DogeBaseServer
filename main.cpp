

#include <vector>
#include <fstream>
#include "Socket.hpp"
#include "DogeType.hpp"


int main(int argc, char const* argv[])
{


    char tmp[256];
    getcwd(tmp, 256);
    std::cout << "Current working directory: " << tmp << std::endl;
    //todo transactions
    //todo organization and classes
    //todo check if index is in range



    while(true){




        Socket s(8080);
        while(true) {
            bool complete = false;
            int op;

            int id;

            DogeObject *obj;

            //one for index, one for object, one for operation
            for (int i = 0; i < 3; i++) {
                //get data
                std::string bytes_in = s.read();
                std::cout << bytes_in << " <- received string \n";
                //check data
                if (s.isDisconnected()) {
                    break;
                }
                if (bytes_in == "stop") {
                    break;
                }
                if (bytes_in == "clear") {
                    std::ofstream clear ("index.dogetable");
                    clear.clear();
                    clear.close();
                    std::ofstream c ("database.dogetable");
                    c .clear();
                    c.close();

                    s.write("k");
                    complete = true;
                    break;
                }

                switch (i) {
                    case 0:
                        op = DogeInt(bytes_in).getValue();
                        s.write("k");
                        break;
                    case 1:
                        id = DogeInt(bytes_in).getValue();
                        if (op == 0) {
                            complete = true;
                        } else {
                            s.write("k");
                        }

                        break;
                    case 2:
                        obj = new DogeObject(bytes_in);
                        complete = true;

                        break;


                }
                if (complete) {
                    if (op == 0) {
                        std::ifstream infile ("database.dogetable");
                        std::cout << "Returned " << id << "\n";

                        std::string str;


                        for (int i = 0; i < id+1; ++i) {

                           char data [4];

                            if(!infile.read( data, 4)){
                                break;
                            };
                            int size = DogeInt(data).getValue();
      //  std::cout << size << " size \n";
                       char data_2 [size];

                            if(!infile.read(data_2,size)){
                                break;
                            };

                            str = std::string (data_2,size);


                        }



                        std::cout << str << "\n";
                        //read
                        s.write(str);
                        infile.close();
                        break;
                    } else {
                        std::ofstream outfile ("database.dogetable",std::ofstream::app);
                        std::cout << "Added \n";
                        //write
                        outfile  <<DogeInt(obj->serialize().size()).serialize() << obj->serialize();


                        outfile.close();
                        std::ifstream infile ("index.dogetable");
                        std::string str = "    ";
                        bool read = false;
                        int num = 0;
                        while (   infile.read( &*str.begin(), 4)) {
                            num++;
                            read = true;

                        }
                            std::cout << "num: " << num << "\n";

                        infile.close();
                        DogeInt index(str);
                        str.clear();
                        if(!read){
                            index = -1;
                        }



                        std::ofstream outfile_2 ("index.dogetable",std::ofstream::app);
                        outfile_2.write(DogeInt(index.getValue() + 1).serialize().data(), 4 );
                        outfile_2.close();
                        std::cout << "id: " << index.getValue() + 1 << " \n";
                        s.write(DogeInt(index.getValue() + 1).serialize());


                    }
                }


            }





            if(!complete){
                break;
            }
/*
 *
 *   //convert data
            DogeObject obj_in = DogeObject( bytes_in);
            //return data
            s.write(obj_in.serialize());
            //convert data
            DogeInt number_in = DogeInt( bytes_in);
            std::cout << number_in.getValue() << " <- received number \n";

            //modify data
            DogeInt number_out = number_in.getValue() + 6;

            //return data
            s.write(number_out.serialize());
            std::cout << number_out.getValue() << " <- sent \n";
*/



        }


    }


}