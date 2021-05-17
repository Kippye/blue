#pragma once

#include "informationheader.hpp"
#include "objecttable.hpp"
#include "commontable.hpp"
#include "datatable.hpp"

namespace blf
{
    struct BLFFile
    {
        // Information about the files.
        const char* path;
        
        // These contain the data in the file.
        InformationHeader  header;
        ObjectTable        objects;
        CommonTable        common;
        DataTable          data;
    };

    void    writeFile(const BLFFile& file);
    void    writeFile(const char* const path, const ObjectTable& objects, DataTable& data);
    void    writeFile(const char* const path, const InformationHeader& header, const ObjectTable& objects, const CommonTable& common, const DataTable& data);
    
    BLFFile readFile(const char* const path, const blf::ObjectTable& objects);

    void    readFile(const char* path, blf::BLFFile& file, const ObjectTable& objects);
    /**
     *  Assumes all required objects are found within the BLFFile object.
     */
    void    readFile(blf::BLFFile& file);
    void    readFile(blf::BLFFile& file, const ObjectTable& objects);
}