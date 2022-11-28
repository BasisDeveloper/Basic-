namespace FileFriend
{
    bool Read_File(const std::filesystem::path& file_path, std::string& contents)
    {
        FILE* file = std::fopen(file_path.string().c_str(), "rb");

        if (!file)
        {
            return false;
        }

        std::fseek(file, 0, SEEK_END);
        size_t file_length = std::ftell(file);
        std::rewind(file);
        char* buffer = new char[file_length];
        std::fread(buffer, sizeof(char), file_length, file);

        // FIXME:QUESTION: Does std::move make std::string own the memory? If not, this is a memory leak...
        std::string file_contents = std::move(buffer);
        file_contents.resize(file_length);
        contents = file_contents;

        std::fclose(file);

        return true;
    }

    bool Write_File(const std::filesystem::path& file_path, const std::string& to_write)
    {
        auto file = std::fopen(file_path.string().c_str(), "wb");

        if (!file)
        {
            return false;
        }

        size_t bytes_written = std::fwrite(
            to_write.data(), sizeof(char), to_write.length(), file);

        if (bytes_written < to_write.length() or bytes_written < 0)
        {
            return false;
        }

        std::fclose(file);

        return true;
    }
}
