#ifndef ENTRYMODEL_H
#define ENTRYMODEL_H

#include <QString>
#include <vector>

struct sEntryModel
{
    std::vector<QString> answers_;
    QString hint_;
    std::vector<QString> questions_;
};

#endif // ENTRYMODEL_H
