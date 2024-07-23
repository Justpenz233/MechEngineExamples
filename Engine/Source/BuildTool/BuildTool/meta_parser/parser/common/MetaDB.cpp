#include "MetaDB.h"
#include <fstream>

std::string NormalPath(std::string Path)
{
	std::filesystem::path p(Path);
	return p.lexically_normal().string();
}

MetaDB::MetaDB(std::string PathToDB)
{
    DBFilePath = NormalPath(PathToDB);
    DBFilePath = DBFilePath / "_generated" / "DB";
    std::ifstream DBFile(DBFilePath);
    std::cout << "Reading CODE database : " << DBFilePath << std::endl;
    if(DBFile.is_open())
    {
        std::string FilePath;
        std::size_t FileTime;
        while(!DBFile.eof()) {
            DBFile >> FilePath >> FileTime;
        	DB[FilePath].FileName = FilePath;
			DB[FilePath].TimeStamp = FileTime;
        	int ClassNum;
        	DBFile >> ClassNum;
        	for(int i = 0; i < ClassNum; i++)
			{
				std::string ClassName;
				DBFile >> ClassName;
				DB[FilePath].ContainClass.insert(ClassName);
			}
        }
        DBFile.close();
    }
}

MetaDB::~MetaDB()
{
    std::ofstream DBFile(DBFilePath);

    if(DBFile.is_open())
    {
        for(auto p : DB)
        {
        	// File name and timestamp
            DBFile << p.second.FileName << " " << p.second.TimeStamp << std::endl;

        	// Contain class
			DBFile << p.second.ContainClass.size() << " ";
        	for(auto i : p.second.ContainClass)
			{
				DBFile << i << " ";
			}
        	DBFile << std::endl;
        }
    	std::cout << "Updating CODE database : " << DBFilePath << std::endl;
    	DBFile.close();
    }
	else
		std::cerr << "Failed to open file : " << DBFilePath << std::endl;
}
template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
              + system_clock::now());
    return system_clock::to_time_t(sctp);
}
bool MetaDB::IsUpdated(std::string FilePath)
{
	std::filesystem::file_time_type ftime = std::filesystem::last_write_time(FilePath);
	std::time_t						t = to_time_t(ftime);

	FilePath = NormalPath(FilePath);
	std::string timeString = std::ctime(&t);
	std::size_t str_hash = std::hash<std::string>{}(timeString);
	if (DB.count(FilePath) == 0 || DB[FilePath].TimeStamp != str_hash)
	{
		DB[FilePath].TimeStamp = str_hash;
		DB[FilePath].FileName = FilePath;
		return true;
	}
	else
		return false;
}
void MetaDB::RegisterClass(std::string ClassName, std::string FilePath)
{
	static int count = 0;
	FilePath = NormalPath(FilePath);
	std::cout << std::setw(3) << ++ count << "| Registering class :  " << std::setw(30) << std::left << ClassName  << "| in | " << FilePath << std::endl;
	DB[FilePath].ContainClass.insert(ClassName);
}
std::vector<std::string> MetaDB::GetAllHeaderPath() const
{
	std::vector<std::string> ret;
	for (auto p : DB)
	{
		ret.push_back(p.first);
	}
	return ret;
}

std::vector<std::string> MetaDB::GetAllShouldCompileHeader() const
{
	std::vector<std::string> ret;
	for (auto p : DB)
	{
		if(!p.second.ContainClass.empty())
			ret.push_back(p.first);
	}
	return ret;
}

void MetaDB::InitSingleton(std::string PathToDB)
{
	Get() = MetaDB(std::move(PathToDB));
}

MetaDB& MetaDB::Get()
{
	static MetaDB instance;
	return instance;
}