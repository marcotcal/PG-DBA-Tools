/*
******************************************************************************

	PGDBATOOLS AGENT

	START  : 2026-08-06
	AUTHOR : Marco Túlio Castro

*******************************************************************************
*/
package db

import (
	"context"
	"log"
	"pgdbagent/internal/config"
	"strconv"

	"github.com/jackc/pgx/v5"
)

type Databases struct {
	Databases []*Database
}

type Database struct {
	Config *config.InstanceConfig
	Conn   *pgx.Conn
}

func (d *Databases) append(cfg *config.InstanceConfig, conn *pgx.Conn) {
	newDatabase := Database{Config: cfg, Conn: conn}
	d.Databases = append(d.Databases, &newDatabase)
}

func ConnectionString(inst *config.InstanceConfig) string {
	var connStr string

	if inst.Password == "" {
		connStr = "postgres://" + inst.User + "@" + inst.Host + ":" + strconv.Itoa(inst.Port) + "/" + inst.Database
	} else {
		connStr = "postgres://" + inst.User + ":" + inst.Password + "@" + inst.Host + ":" +
			strconv.Itoa(inst.Port) + "/" + inst.Database
	}

	return connStr
}

func Connect(ctx context.Context, cfg *config.Config) (databases *Databases) {

	var majorVersion string
	data := &Databases{}

	for index, instance := range cfg.Instances {
		conn, err := pgx.Connect(ctx, ConnectionString(&cfg.Instances[index]))
		if err != nil {
			log.Printf("Error connecting in %s: %v", instance.Name, err)
			continue
		}

		err = conn.QueryRow(
			ctx,
			"SELECT split_part(current_setting('server_version'), '.', 1);",
		).Scan(&majorVersion)

		if err != nil {
			log.Fatal(err)
		}

		cfg.Instances[index].MajorVersion = majorVersion

		data.append(&cfg.Instances[index], conn)

	}
	return data
}
