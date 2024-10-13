target_sources(${PROJECT_NAME}
    PRIVATE
    main.cpp
    scheduler.cpp
    job_gen.cpp
)

target_sources(job_gen
    PRIVATE
    job_gen_main.cpp
    job_gen.cpp
)
