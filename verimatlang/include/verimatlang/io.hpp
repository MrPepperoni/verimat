#pragma once

namespace verimatlang {

class input_bitstream {
public:
    virtual ~input_bitstream();
    virtual bool read() = 0;
};

class output_bitstream {
public:
    virtual ~output_bitstream();
    virtual void write(bool bit) = 0;
    virtual void flush() = 0;
};

}
