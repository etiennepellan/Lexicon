#include "listio.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QFileInfo>

const QString ListIO::json_name_str_ = "name";
const QString ListIO::json_instructions_str_ = "instructions";
const QString ListIO::json_entries_str_ = "entries";
const QString ListIO::json_questions_str_ = "questions";
const QString ListIO::json_answers_str_ = "answers";
const QString ListIO::json_hint_str_ = "hint";

ListIO::ListIO() {}

size_t ListIO::Merge(
        const QString &dir_path,
        const QString &file_path,
        QString &error_msg)
{
    QStringList lists = QDir(dir_path).entryList(QStringList() << "*.json" << "*.JSON", QDir::Files);
    size_t merged_lists = 0;
    sListData data;

    for(QString &list : lists)
    {
        list.prepend(dir_path + '/');

        QString load_err_msg;
        const size_t prv_entry_count = data.entries_.size();
        const bool load_ret = Load(list, data, load_err_msg);
        const size_t entry_count = data.entries_.size();

        if(load_ret) merged_lists++;
        else
        {
            error_msg += QFileInfo(list).fileName() + " : " + load_err_msg;
            auto const it = data.entries_.begin();
            data.entries_.erase(it + prv_entry_count - 1, it + entry_count - 1); // delete entries from invalid list
        }
    }

    if(merged_lists)
    {
        const bool save_ret = Save(file_path, data, error_msg);
        merged_lists = save_ret ? merged_lists : 0;
    }

    return merged_lists;
}

bool ListIO::Load(const QString &fpath, sListData &data, QString &error_msg)
{
    if(!fpath.isEmpty())
    {
        QFile file(fpath);

        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray bytes = file.readAll();
            file.close();

            QJsonParseError json_error;
            QJsonDocument json_doc = QJsonDocument::fromJson(bytes, &json_error);

            if(json_error.error != QJsonParseError::NoError)
            {
                error_msg += "JSON parsing failed with error :\r\n" + json_error.errorString();
                return false;
            }

            if(json_doc.isObject())
            {
                QJsonObject json_content = json_doc.object();

                if( !json_content.contains(json_name_str_) ||
                    !json_content.contains(json_instructions_str_) ||
                    !json_content.contains(json_entries_str_))
                {
                    error_msg += "List is missing one or several parameters\r\n";
                    return false;
                }

                data.name_ = json_content.value(json_name_str_).toString();
                data.instructions_ = json_content.value(json_instructions_str_).toString();

                QJsonArray json_entries_array = json_content.value(json_entries_str_).toArray();

                const size_t json_entries_count = json_entries_array.size();

                for(size_t i = 0; i < json_entries_count; ++i)
                {
                    QJsonObject entry_obj = json_entries_array.at(i).toObject();

                    if( !entry_obj.contains(json_questions_str_) ||
                        !entry_obj.contains(json_answers_str_) ||
                        !entry_obj.contains(json_hint_str_))
                    {
                        error_msg += "List has one or several invalid entries\r\n";
                        return false;
                    }

                    sEntryModel entry;
                    entry.hint_ = entry_obj.value(json_hint_str_).toString();

                    const QJsonArray entry_questions_array = entry_obj.value(json_questions_str_).toArray();
                    const size_t entry_questions_count = entry_questions_array.size();

                    for(size_t i = 0; i < entry_questions_count; ++i)
                    {
                        const QString question = entry_questions_array.at(i).toString();
                        entry.questions_.push_back(question);
                    }

                    const QJsonArray entry_answers_array = entry_obj.value(json_questions_str_).toArray();
                    const size_t entry_answers_count = entry_answers_array.size();

                    for(size_t i = 0; i < entry_answers_count; ++i)
                    {
                        const QString answer = entry_answers_array.at(i).toString();
                        entry.questions_.push_back(answer);
                    }

                    data.entries_.push_back(entry);
                }
            }
         }

        else
        {
            error_msg += "Could not open file\r\n";
            return false;
        }
    }

    return true;
}

bool ListIO::Save(
    const QString &fpath,
    sListData &data,
    QString &error_msg)
{
    if(!fpath.isEmpty())
    {
        QJsonArray json_entry_array;

        for(const auto& entry : data.entries_)
        {
            QJsonArray json_entry_questions;

            for(const auto& question : entry.questions_)
            {
                json_entry_questions.append(question);
            }

            QJsonArray json_entry_answers;

            for(const auto& answer : entry.answers_)
            {
                json_entry_answers.append(answer);
            }

            QJsonObject json_entry;
            json_entry.insert(json_questions_str_, json_entry_questions);
            json_entry.insert(json_answers_str_, json_entry_answers);
            json_entry.insert(json_hint_str_, entry.hint_);

            json_entry_array.append(json_entry);
        }

        QJsonObject json_content;
        json_content.insert(json_name_str_, data.name_);
        json_content.insert(json_instructions_str_, data.instructions_);
        json_content.insert(json_entries_str_, json_entry_array);

        QJsonDocument json_doc;
        json_doc.setObject(json_content);

        const QByteArray bytes = json_doc.toJson(QJsonDocument::Indented);

        QFile file(fpath);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            QTextStream iStream(&file);
            iStream.setCodec("utf-8");
            iStream << bytes;
            file.close();
        }

        else
        {
            error_msg += "Could not open file\r\n";
            return false;
        }
    }

    return true;
}
