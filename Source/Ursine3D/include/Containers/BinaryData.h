/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BinaryData.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    class BinaryData
    {
    public:
        typedef std::shared_ptr<BinaryData> Handle;
        typedef std::unique_ptr<byte[]> Data;

        BinaryData(void);
        BinaryData(BinaryData &&rhs);
        BinaryData(void *data, size_t size);

        BinaryData &operator=(BinaryData &&rhs);

        // Implicit copies are not permitted, so use this interface if needed
        static BinaryData Copy(const BinaryData &rhs);

        // Transfers ownership of "data" to "input" without copying the
        // memory associated
        static void Load(BinaryData &input, void *data, size_t size);

        const void *GetData(void) const;
        size_t GetSize(void) const;

        // copies the memory of this data to the destination
        void CopyTo(void *destination) const;

    private:
        Data m_data;
        size_t m_size;

        BinaryData(const BinaryData &rhs) = delete;
        BinaryData &operator=(const BinaryData &rhs) = delete;
    };
}