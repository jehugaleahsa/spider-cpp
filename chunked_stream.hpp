#include <algorithm>
#include <iostream>
#include <memory>
#include <boost/iostreams/stream.hpp>

namespace {

    template <typename CharT, typename Traits = std::char_traits<CharT>>
    class ChunkedStreamSource {
        std::shared_ptr<std::basic_istream<CharT, Traits>> m_stream;
        std::streamsize m_chunkSize;
        std::streamsize m_position;

    public:
        typedef CharT char_type;
        typedef boost::iostreams::source_tag category;

        ChunkedStreamSource(std::shared_ptr<std::basic_istream<CharT, Traits>> stream) 
            : m_stream(stream), m_chunkSize(), m_position() {
        }

        std::streamsize read(char * buffer, std::streamsize n) {
            using std::copy;
            using std::getline;
            using std::hex;
            using std::min;
            using std::streamsize;
            using std::string;

            if (m_position == m_chunkSize) {
                *m_stream >> hex >> m_chunkSize;
                m_position = 0;
                string ignore;
                getline(*m_stream, ignore);
            }
            streamsize remaining = m_chunkSize - m_position;
            streamsize count = min(remaining, n);
            m_stream->read(buffer, count);
            m_position += count;
            return count;
        }
    };

}

namespace spider {

    template <typename CharT, typename Traits = std::char_traits<CharT>>
    class ChunkedStream : public virtual boost::iostreams::stream<ChunkedStreamSource<CharT, Traits>> {
    public:
        ChunkedStream(std::shared_ptr<std::basic_istream<CharT, Traits>> stream)
            : boost::iostreams::stream<ChunkedStreamSource<CharT, Traits>>((ChunkedStreamSource<CharT, Traits>(stream))) {
        }
    };

}
