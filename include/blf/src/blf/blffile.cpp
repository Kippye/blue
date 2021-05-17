#include "blf/blffile.hpp"

#include "blf/version.hpp"

#include "blf/writer.hpp"
#include "blf/reader.hpp"

#include <chrono>

namespace blf
{
    void writeFile(const BLFFile& file)
    {
        writeFile(file.path, file.header, (const ObjectTable&)file.objects, file.common, file.data);
    }

    void writeFile(const char* const path, const ObjectTable& objects, DataTable& data)
    {
        InformationHeader header = {BLF_SIGNATURE, VERSION_MAJOR, VERSION_MINOR, VERSION_FIX};
        CommonTable common;

        data.computeCommonTable(common, objects);
        data.buildArray();
        common.buildCommonObjectArray();

        writeFile(path, header, objects, common, data);
    }

    void writeFile(const char* const path, const InformationHeader& header, const ObjectTable& objects, const CommonTable& common, const DataTable& data)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        blf::Writer writer(path);

        //writer.storeObjectTable(objects);

        //writer.storeCommonTable(common);
        std::cout << "BLF: STARTING WRITE" << std::endl;
        writer.writeInformationHeader(header);
        writer.writeObjectTable(objects);
        std::cout << "COMM" << std::endl;
        writer.writeCommonTable(common, objects);
        std::cout << "DAT" << std::endl;
        writer.writeDataTable(data, objects, common);
        std::cout << "TIM" << std::endl;
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

        std::cout << "Write completed in: " << duration << " milliseconds." << std::endl;

        std::cout << "Common objects:" << std::endl;
        if (common.isArrayBuilt() && common.getArraySize() > 0)
        {
            for (blf::TemplateObject* tex : common)
            {
                for (blf::ObjectAttribute attrib : tex->getAttributeMap())
                {
                    std::cout << attrib.name << ": ";
                    switch (attrib.attribType)
                    {
                        case blf::TYPE_STRING:
                            blf::String* string = (blf::String*)attrib.offset;
                            std::cout << *string;
                            break;
                    }
                    std::cout << std::endl;
                }
            }
        }
        else
        {
            std::cout << "none" << std::endl;
        }
    }

    BLFFile readFile(const char* const path, const blf::ObjectTable& objects)
    {
        BLFFile file;
        file.path    = path;
        file.objects = objects;

        readFile(file);

        return file;
    }

    void readFile(blf::BLFFile& file)
    {
        auto t3 = std::chrono::high_resolution_clock::now();
        blf::Reader levelReader(file.path);
        file.header = levelReader.dynamicRead<blf::InformationHeader>();

        levelReader.readObjectTable(file.objects);
        
        file.common = levelReader.readCommonTable(file.objects);
        file.common.buildCommonObjectArray();

        blf::DataTable data;
        levelReader.readDataTable(&data, file.objects, file.common);
        data.buildArray();
        file.data = data;


        blf::ObjectDefinition* definition = file.objects.getDefinitionFromIndex(0);
        /*for( int i = 0; i < file.objects.getSize(); i++, definition = file.objects.getDefinitionFromIndex(i))
        {
            std::cout << ((definition->isForeign) ? "[F] " : "") << definition->identifier << std::endl;
            for( blf::ObjectAttribute attribute : definition->attributes )
            {
                std::cout << "- " << ((attribute.isForeign | definition->isForeign) ? "[F] " : "") << attribute.name << 
                ((attribute.isActive) ? ": Active!" : "" ) << std::endl;
                std::cout << "-> " << (int)getTypeSize(attribute.attribType) << std::endl;
                std::cout << "-> " << (unsigned long long)attribute.offset << std::endl;
            }
        }*/

        auto t4 = std::chrono::high_resolution_clock::now();
        auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
        std::cout << "Read completed in: " << duration2 << " milliseconds." << std::endl;
    }

    void readFile(blf::BLFFile& file, const ObjectTable& objects)
    {
        file.objects = objects;
        readFile(file);
    }

    void readFile(const char* path, blf::BLFFile& file, const ObjectTable& objects)
    {
        file.objects = objects;
        file.path    = path;
        readFile(file);
    }
}