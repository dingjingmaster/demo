/*************************************************************************
> FileName: cups-pages.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年05月21日 星期三 14时00分05秒
 ************************************************************************/
#include <cups/cups.h>
#include <stdio.h>

int get_job_page_count(int job_id, const char *printer_name) 
{
    ipp_t *request, *response;
    ipp_attribute_t *attr;
    int page_count = -1;
    char job_uri[1024];

    // 构造作业的 URI
    snprintf(job_uri, sizeof(job_uri), "ipp://localhost/jobs/%d", job_id);

    // 创建 IPP 请求
    request = ippNewRequest(IPP_GET_JOB_ATTRIBUTES);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, printer_name);
    ippAddInteger(request, IPP_TAG_OPERATION, IPP_TAG_INTEGER, "job-id", job_id);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "job-uri", NULL, job_uri);

    // 发送请求
    response = cupsDoRequest(CUPS_HTTP_DEFAULT, request, "/jobs");

    if (response != NULL) {
        // 查找 job-impressions 属性
        attr = ippFindAttribute(response, "job-impressions", IPP_TAG_INTEGER);
        if (attr != NULL) {
            page_count = ippGetInteger(attr, 0);
            printf("Estimated page count for job %d: %d\n", job_id, page_count);
        } else {
            fprintf(stderr, "No page count information available for job %d.\n", job_id);
        }
        ippDelete(response);
    } else {
        fprintf(stderr, "Error querying job attributes: %s\n", cupsLastErrorString());
    }

    return page_count;
}

int main(int argc, char* argv[]) 
{
    if (argc != 3) {
        printf ("Usage: %s <printer name> <pdf path>\n", argv[0]);
        return -1;
    }
    const char *printer_name = argv[1]; //"MyPrinter";
    const char *filename = argv[2]; //"example.pdf";
    int job_id;
    cups_job_t *jobs;
    int num_jobs;

    // 提交打印作业
    job_id = cupsPrintFile(printer_name, filename, "Test Job", 0, NULL);
    if (job_id <= 0) {
        fprintf(stderr, "Error submitting print job: %s\n", cupsLastErrorString());
        return 1;
    }
    printf("Print job submitted with ID: %d\n", job_id);

    // 获取作业列表
    num_jobs = cupsGetJobs(&jobs, printer_name, 0, CUPS_WHICHJOBS_ACTIVE);
    if (num_jobs <= 0) {
        fprintf(stderr, "No jobs found: %s\n", cupsLastErrorString());
        return 1;
    }

    // 查找目标作业并获取页数
    for (int i = 0; i < num_jobs; i++) {
        if (jobs[i].id == job_id) {
            printf("Found job: %d, Title: %s, State: %d\n", jobs[i].id, jobs[i].title, jobs[i].state);
            get_job_page_count(job_id, printer_name);
            break;
        }
    }

    // 释放作业列表
    cupsFreeJobs(num_jobs, jobs);

    return 0;
}
