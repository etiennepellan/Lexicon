#ifndef LISTIO_H
#define LISTIO_H

#include <vector>
#include "entrymodel.h"

class ListIO
{
public:

    struct sListData
    {
        QString name_;
        QString instructions_;
        std::vector<EntryModel> entries_;
    };

    static size_t Merge(
        const QString &dir_path,
        const QString &file_path,
        QString &error_msg
    );

    static bool Load(
        const QString &fpath,
        sListData& data,
        QString &error_msg
    );

    static bool Save(
        const QString &fpath,
        sListData& data,
        QString &error_msg
    );

private:

    ListIO();

    static const QString json_name_str_;
    static const QString json_instructions_str_;
    static const QString json_entries_str_;
    static const QString json_question_str_;
    static const QString json_answer_str_;
    static const QString json_hint_str_;
};

#endif // LISTIO_H
