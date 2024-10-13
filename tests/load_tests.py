from locust import HttpUser, between, task

class WebUser(HttpUser):
    wait_time = between(1, 5)

    @task
    def load_index(self):
        self.client.get("/index.html")

    @task
    def load_prueba(self):
        self.client.get("/prueba.html")

    @task
    def load_sitio1(self):
        self.client.get("/sitio1/index.html")

    @task
    def load_sitio2(self):
        self.client.get("/sitio2/index.html")

    @task
    def load_sitio3(self):
        self.client.get("/sitio3/index.html")
