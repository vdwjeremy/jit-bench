#include <iostream>
#include <elfio/elfio.hpp>

using namespace ELFIO;


class ElfReader {
protected:
    elfio reader;

public:
    ElfReader(const char* file) {
        // Load ELF data
        if ( !reader.load( file ) ) {
            std::cerr << "Can't find or process ELF file " << std::endl;
        }
    }

    void load(auto loader) {
        // Print ELF file segments info
        Elf_Half seg_num = reader.segments.size();
        for ( int i = 0; i < seg_num; ++i ) {
            const segment* pseg = reader.segments[i];
            if (pseg->get_type() == PT_LOAD) {
                loader(pseg->get_data(), pseg->get_file_size(), pseg->get_virtual_address());
            }
        }
    }

    Elf64_Addr getSymbolAddress(std::string symbolName) {
        Elf_Half sec_num = reader.sections.size();
        for ( int i = 0; i < sec_num; ++i ) {
            section* psec = reader.sections[i];
            // Check section type
            if ( psec->get_type() == SHT_SYMTAB ) {
                const symbol_section_accessor symbols( reader, psec );
                for ( unsigned int j = 0; j < symbols.get_symbols_num(); ++j ) {
                    std::string   name;
                    Elf64_Addr    value;
                    Elf_Xword     size;
                    unsigned char bind;
                    unsigned char type;
                    Elf_Half      section_index;
                    unsigned char other;

                    // Read symbol properties
                    symbols.get_symbol( j, name, value, size, bind, type,
                                        section_index, other );
                    if (symbolName == name){
                        return value;
                    }
                }
            }
        }
        return 0;
    }

};




